
#include "../include/CaptureManager.h"
#include "../include/LayoutManager.h"
#include "../include/Logger.h"
#include "../include/CompetitionDetector.h"

#include <opencv2/imgproc.hpp>
#include <windows.h>
#include <chrono>

cv::Mat CaptureManager::CaptureScreen()
{
    HWND hwnd = GetDesktopWindow();

    if (!hwnd)
    {
        Logger::Error("Could not get desktop window");
        return {};
    }

    HDC screenDC = GetDC(hwnd);

    if (!screenDC)
    {
        Logger::Error("Could not get desktop DC");
        return {};
    }

    HDC memoryDC = CreateCompatibleDC(screenDC);

    if (!memoryDC)
    {
        Logger::Error("Could not create compatible DC");
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    RECT screenRect{};

    if (!GetClientRect(hwnd, &screenRect))
    {
        Logger::Error("Could not get desktop dimensions");
        DeleteDC(memoryDC);
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    int width = screenRect.right;
    int height = screenRect.bottom;

    if (width <= 0 || height <= 0)
    {
        Logger::Error("Invalid desktop dimensions");
        DeleteDC(memoryDC);
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    HBITMAP bitmap = CreateCompatibleBitmap(
        screenDC,
        width,
        height
    );

    if (!bitmap)
    {
        Logger::Error("Could not create desktop bitmap");
        DeleteDC(memoryDC);
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    HGDIOBJ oldBitmap = SelectObject(memoryDC, bitmap);

    if (!oldBitmap || oldBitmap == HGDI_ERROR)
    {
        Logger::Error("Could not select desktop bitmap");
        DeleteObject(bitmap);
        DeleteDC(memoryDC);
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    BOOL copied = BitBlt(
        memoryDC,
        0,
        0,
        width,
        height,
        screenDC,
        0,
        0,
        SRCCOPY
    );

    SelectObject(memoryDC, oldBitmap);

    if (!copied)
    {
        Logger::Error("Screen BitBlt failed");
        DeleteObject(bitmap);
        DeleteDC(memoryDC);
        ReleaseDC(hwnd, screenDC);
        return {};
    }

    BITMAPINFO bitmapInfo{};
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = width;
    bitmapInfo.bmiHeader.biHeight = -height;
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    cv::Mat bgra(height, width, CV_8UC4);

    int copiedLines = GetDIBits(
        screenDC,
        bitmap,
        0,
        height,
        bgra.data,
        &bitmapInfo,
        DIB_RGB_COLORS
    );

    DeleteObject(bitmap);
    DeleteDC(memoryDC);
    ReleaseDC(hwnd, screenDC);

    if (copiedLines != height)
    {
        Logger::Error("Could not read all desktop bitmap lines");
        return {};
    }

    cv::Mat bgr;

    cv::cvtColor(
        bgra,
        bgr,
        cv::COLOR_BGRA2BGR
    );

    return bgr;
}


bool CaptureManager::Capture(
    const std::string& imagePath,
    const std::string& competition,
    cv::Mat& grayImage,
    cv::Mat& binaryImage)
{
    grayImage.release();
    binaryImage.release();
    lastFrame.release();
    detectedCompetition.clear();

    auto totalStart = std::chrono::steady_clock::now();

    cv::Mat image;

    //----------------------------------------------------
    // 1. Image Acquisition
    //----------------------------------------------------

    if (imagePath.empty())
    {
        Logger::Info("Capture Mode : LIVE SCREEN");

        auto captureStart = std::chrono::steady_clock::now();

        image = CaptureScreen();

        auto captureMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - captureStart
            ).count();

        Logger::Info(
            "[PERF] Screen Capture: " +
            std::to_string(captureMs) + " ms"
        );

        if (image.empty())
        {
            Logger::Error("Failed to capture screen");
            return false;
        }

        // Temporary Windows/browser testing adjustment.
        // This is not the final PlayStation screen layout.
        if (image.rows > 40)
        {
            cv::Rect browserRect(
                0,
                0,
                image.cols,
                image.rows - 40
            );

            image = image(browserRect).clone();
        }
    }
    else
    {
        Logger::Info("Capture Mode : TEST IMAGE");

        auto loadStart = std::chrono::steady_clock::now();

        image = cv::imread(imagePath);

        auto loadMs =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - loadStart
            ).count();

        Logger::Info(
            "[PERF] Image Load: " +
            std::to_string(loadMs) + " ms"
        );
    }

    if (image.empty())
    {
        Logger::Error("Failed to load image");
        return false;
    }

    //----------------------------------------------------
    // 2. Preserve Full Screen Image
    //----------------------------------------------------

    lastFrame = image.clone();

    Logger::Info(
        "Screen Width = " + std::to_string(image.cols)
    );

    Logger::Info(
        "Screen Height = " + std::to_string(image.rows)
    );

    Logger::Info("Full Screen Image Saved");

    //----------------------------------------------------
    // 3. Competition Detection
    //----------------------------------------------------

    auto competitionStart = std::chrono::steady_clock::now();

    detectedCompetition = CompetitionDetector::Detect(image);

    auto competitionMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - competitionStart
        ).count();

    Logger::Info(
        "[PERF] Competition Detection: " +
        std::to_string(competitionMs) + " ms"
    );

    //----------------------------------------------------
    // 4. Competition Validation
    //----------------------------------------------------

    if (detectedCompetition.empty() ||
        detectedCompetition == "Unknown")
    {
        Logger::Info(
            "Competition unknown. Full screen remains available."
        );

        return false;
    }

    Logger::Info(
        "Competition : " + detectedCompetition
    );

    //----------------------------------------------------
    // 5. Timer Region
    //----------------------------------------------------

    auto processingStart = std::chrono::steady_clock::now();

    cv::Rect timerRegion =
        LayoutManager::GetTimerRegion(detectedCompetition);

    if (timerRegion.width <= 0 ||
        timerRegion.height <= 0 ||
        timerRegion.x < 0 ||
        timerRegion.y < 0 ||
        timerRegion.x + timerRegion.width > image.cols ||
        timerRegion.y + timerRegion.height > image.rows)
    {
        Logger::Error(
            "Invalid timer region for competition: " +
            detectedCompetition
        );

        return false;
    }

    cv::Mat timer = image(timerRegion);

    if (timer.empty())
    {
        Logger::Error("Timer region is empty");
        return false;
    }

    //----------------------------------------------------
    // 6. Grayscale
    //----------------------------------------------------

    cv::Mat gray;

    cv::cvtColor(
        timer,
        gray,
        cv::COLOR_BGR2GRAY
    );

    //----------------------------------------------------
    // 7. Resize
    //----------------------------------------------------

    cv::resize(
        gray,
        gray,
        cv::Size(),
        6.0,
        6.0,
        cv::INTER_LINEAR
    );

    //----------------------------------------------------
    // 8. Binary Processing
    //----------------------------------------------------

    cv::Mat binary;

    int thresholdType = cv::THRESH_BINARY;

    if (detectedCompetition == "SerieA")
    {
        thresholdType = cv::THRESH_BINARY_INV;
    }

    cv::threshold(
        gray,
        binary,
        120,
        255,
        thresholdType
    );

    if (gray.empty() || binary.empty())
    {
        Logger::Error("Timer image processing failed");
        return false;
    }

    //----------------------------------------------------
    // 9. Return Timer Images
    //----------------------------------------------------

    grayImage = gray;
    binaryImage = binary;

    auto processingMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - processingStart
        ).count();

    auto totalMs =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now() - totalStart
        ).count();

    Logger::Info(
        "[PERF] Timer Processing: " +
        std::to_string(processingMs) + " ms"
    );

    Logger::Info(
        "[PERF] Capture Pipeline Total: " +
        std::to_string(totalMs) + " ms"
    );

    Logger::Info("Timer Images Ready in Memory");

    return true;
}


std::string CaptureManager::GetCompetition() const
{
    return detectedCompetition;
}


cv::Mat CaptureManager::GetLastFrame() const
{
    return lastFrame.clone();
}
