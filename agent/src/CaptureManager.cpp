#include "../include/CaptureManager.h"
#include "../include/Logger.h"

#include <opencv2/opencv.hpp>

void CaptureManager::Capture()
{
    cv::Mat image = cv::imread("Debug/fifa_test.jpg");

    if (image.empty())
    {
        Logger::Error("Failed to load image");
        return;
    }

    Logger::Info("Image Loaded Successfully");

    cv::Rect timerRegion(0, 0, 350, 120);

    cv::Mat timerCrop = image(timerRegion);

    cv::Mat gray;
    cv::cvtColor(timerCrop, gray, cv::COLOR_BGR2GRAY);

    cv::Mat enlarged;
    cv::resize(
        gray,
        enlarged,
        cv::Size(),
        3.0,
        3.0,
        cv::INTER_CUBIC);

    cv::Mat binary;
    cv::threshold(
        enlarged,
        binary,
        170,
        255,
        cv::THRESH_BINARY);

    cv::imwrite("timer_binary.jpg", binary);

    cv::imwrite("timer_crop.jpg", timerCrop);

    Logger::Info("Timer Crop Saved");
}