
#pragma once

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include <string>

class FullTimeDetector
{
public:
    FullTimeDetector();
    ~FullTimeDetector();

    bool Detect(const cv::Mat& screenImage);

private:
    std::string ReadText(const cv::Mat& image);

    bool IsFullTimeText(const std::string& text);

    tesseract::TessBaseAPI tess;
    bool initialized = false;

    int confirmationCount = 0;
    static constexpr int REQUIRED_CONFIRMATIONS = 3;
};

