#pragma once

#include <opencv2/opencv.hpp>

#include <string>

class CompetitionDetector
{
public:
    static std::string Detect(
        const cv::Mat &image);
};