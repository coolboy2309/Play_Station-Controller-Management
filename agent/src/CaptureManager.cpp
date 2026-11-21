#include "../include/CaptureManager.h"
#include "../include/LayoutManager.h"
#include "../include/Logger.h"
#include <opencv2/opencv.hpp>
#include "../include/CompetitionDetector.h"
#include <opencv2/imgproc.hpp>
#include <windows.h>

cv::Mat CaptureManager::CaptureScreen()
{
    HWND hwnd = GetDesktopWindow();

    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);

    RECT windowsize;
    GetClientRect(hwnd, &windowsize);

    int width  = windowsize.right;
    int height = windowsize.bottom;

    HBITMAP hbwindow = CreateCompatibleBitmap(
        hwindowDC,
        width,
        height);

    SelectObject(hwindowCompatibleDC, hbwindow);

    BitBlt(
        hwindowCompatibleDC,
        0,
        0,
        width,
        height,
        hwindowDC,
        0,
        0,
        SRCCOPY);

    BITMAPINFOHEADER bi;

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    cv::Mat mat(height, width, CV_8UC4);

    cv::Mat bgr;


    GetDIBits(
        hwindowCompatibleDC,
        hbwindow,
        0,
        height,
        mat.data,
        (BITMAPINFO*)&bi,
        DIB_RGB_COLORS);

    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    cv::cvtColor(mat, bgr, cv::COLOR_BGRA2BGR);

    return bgr;
}

void CaptureManager::Capture(
    const std::string& imagePath,
    const std::string& competition)
{
    cv::Mat image;

    if (imagePath.empty())
    {
        Logger::Info("Capture Mode : LIVE SCREEN");

        image = CaptureScreen();

        if (image.empty())
        {
            Logger::Error("Failed to capture screen");
            return;
        }

        // Remove bottom Windows/taskbar area.
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

        Logger::Info(
            "Screen Width  = " +
            std::to_string(image.cols));

        Logger::Info(
            "Screen Height = " +
            std::to_string(image.rows));
    }
    else
    {
        Logger::Info("Capture Mode : TEST IMAGE");

        image = cv::imread(imagePath);
    }

    if (image.empty())
    {
        Logger::Error("Failed to load image");
        return;
    }

    Logger::Info("Image Loaded Successfully");

    //----------------------------------------------------
    // Competition Detection
    //----------------------------------------------------

    detectedCompetition =
        CompetitionDetector::Detect(image);

    Logger::Info(
        "Competition : " +
        detectedCompetition);

    //----------------------------------------------------
    // Timer Region
    //----------------------------------------------------

    cv::Rect timerOnly =
        LayoutManager::GetTimerRegion(
            detectedCompetition);

    if (timerOnly.width <= 0 ||
        timerOnly.height <= 0 ||
        timerOnly.x < 0 ||
        timerOnly.y < 0 ||
        timerOnly.x + timerOnly.width > image.cols ||
        timerOnly.y + timerOnly.height > image.rows)
    {
        Logger::Error(
            "Invalid timer region for competition: " +
            detectedCompetition);

        return;
    }

    cv::Mat timer =
        image(timerOnly).clone();

    if (timer.empty())
    {
        Logger::Error("Timer region is empty");
        return;
    }

    //----------------------------------------------------
    // Gray
    //----------------------------------------------------

    cv::Mat gray;

    cv::cvtColor(
        timer,
        gray,
        cv::COLOR_BGR2GRAY);

    //----------------------------------------------------
    // Resize
    //----------------------------------------------------

    cv::resize(
        gray,
        gray,
        cv::Size(),
        6.0,
        6.0,
        cv::INTER_LINEAR);

    //----------------------------------------------------
    // Binary processing
    //----------------------------------------------------

    cv::Mat binary;

    if (detectedCompetition == "SerieA")
    {
        cv::threshold(
            gray,
            binary,
            120,
            255,
            cv::THRESH_BINARY_INV);
    }
}

std::string CaptureManager::GetCompetition() const
{
    return detectedCompetition;
}