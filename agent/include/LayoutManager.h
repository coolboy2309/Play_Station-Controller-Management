#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class LayoutManager
{
public:
    static cv::Rect GetTimerRegion(const std::string& competition);
};