#include "../include/CaptureManager.h"
#include "../include/LayoutManager.h"
#include "../include/Logger.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>

void CaptureManager::Capture(const std::string &competition)
{
    cv::Mat image = cv::imread("Debug/fifa_test.jpg");

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

    cv::Rect timerOnly =
        LayoutManager::GetTimerRegion(competition);

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

    if (competition == "SerieA")
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