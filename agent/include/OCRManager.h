#pragma once

#include "TimerStabilizer.h"
#include <string>
#include <opencv2/opencv.hpp>

class OCRManager
{
public:

    std::string ReadTimer(
        const cv::Mat& grayImage,
        const cv::Mat& binaryImage);

private:

    std::string ReadSingleImage(
        const cv::Mat& image,
        int& confidence);

    TimerStabilizer stabilizer;
};