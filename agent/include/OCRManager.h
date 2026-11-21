
#pragma once

#include "TimerStabilizer.h"

#include <string>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

class OCRManager
{
public:
    OCRManager();
    ~OCRManager();

    std::string ReadTimer(
        const cv::Mat& grayImage,
        const cv::Mat& binaryImage
    );

private:
    std::string ReadSingleImage(
        const cv::Mat& image,
        int& confidence
    );

    tesseract::TessBaseAPI tess;
    bool initialized = false;

    TimerStabilizer stabilizer;
};
