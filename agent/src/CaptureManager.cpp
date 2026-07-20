#include "../include/CaptureManager.h"
#include "../include/OCRManager.h"
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

    // Whole scoreboard
    cv::Rect timerRegion(0, 0, 350, 120);
    cv::Mat timerCrop = image(timerRegion);

    // Only the match timer (00:47)
    cv::Rect timerOnly(
        82,
        8,
        135,
        65);
    cv::Mat timer = timerCrop(timerOnly);

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(timer, gray, cv::COLOR_BGR2GRAY);

    // Enlarge image for OCR
    cv::Mat enlarged;
    cv::resize(
        gray,
        enlarged,
        cv::Size(),
        4.0,
        4.0,
        cv::INTER_CUBIC);

    cv::imwrite("timer_binary.jpg", enlarged);

    Logger::Info("Timer Crop Saved");

    // OCRManager ocr;

    // std::string timerText = ocr.ReadTimer("timer_binary.jpg");

    // Logger::Info("OCR : " + timerText);
}