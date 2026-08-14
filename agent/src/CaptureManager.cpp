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

void CaptureManager::Capture(const std::string &imagePath,
                             const std::string &competition)
{
    cv::Mat image;

if (imagePath.empty())
{
    Logger::Info("Capture Mode : LIVE SCREEN");

    image = CaptureScreen();
    cv::imwrite("01_desktop.jpg", image);
    cv::Rect browserRect(
    0,
    0,
    image.cols,
    image.rows - 40
);

image = image(browserRect).clone();

cv::imwrite("02_browser_crop.jpg", image);



    Logger::Info("Screen Width  = " + std::to_string(image.cols));
Logger::Info("Screen Height = " + std::to_string(image.rows));

cv::imwrite("desktop_capture.jpg", image);


}
else
{
    Logger::Info("Capture Mode : TEST IMAGE");

    image = cv::imread(imagePath);
}

    // cv::namedWindow("Debug", cv::WINDOW_NORMAL);

    // cv::setMouseCallback(
    //     "Debug",
    //     [](int event, int x, int y, int, void *)
    //     {
    //         if (event == cv::EVENT_MOUSEMOVE)
    //         {
    //             std::cout << "X=" << x
    //                       << " Y=" << y
    //                       << std::endl;
    //         }
    //     });

    // cv::imshow("Debug", image);
    // cv::waitKey(0);

    if (image.empty())
    {
        Logger::Error("Failed to load image");
        return;
    }

    Logger::Info("Image Loaded Successfully");

    //----------------------------------------------------
    // Get timer position from LayoutManager
    //----------------------------------------------------

    detectedCompetition =
        CompetitionDetector::Detect(image);

    Logger::Info(
        "Competition : " + detectedCompetition);

    cv::Rect timerOnly =
        LayoutManager::GetTimerRegion(detectedCompetition);

    cv::Mat timer =
        image(timerOnly).clone();

    //----------------------------------------------------
    // Gray
    //----------------------------------------------------

    cv::Mat gray;

    cv::cvtColor(
        timer,
        gray,
        cv::COLOR_BGR2GRAY);

    //----------------------------------------------------
    // Resize x6
    //----------------------------------------------------

    cv::resize(
        gray,
        gray,
        cv::Size(),
        6.0,
        6.0,
        cv::INTER_LINEAR);

    //----------------------------------------------------
    // Save
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

        // Save BOTH images from the threshold result
        cv::imwrite("timer_gray.jpg", binary);
        cv::imwrite("timer_binary.jpg", binary);
    }
    else
    {
        // Normal competitions
        cv::imwrite("timer_gray.jpg", gray);
        cv::imwrite("timer_binary.jpg", gray);
    }
}

std::string CaptureManager::GetCompetition() const
{
    return detectedCompetition;
}