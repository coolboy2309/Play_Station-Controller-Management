#pragma once
#include <string>
#include <opencv2/opencv.hpp>

class CaptureManager
{
public:

    void Capture(const std::string& imagePath,
             const std::string& competition);

    std::string GetCompetition() const;

private:

    std::string detectedCompetition;

    cv::Mat CaptureScreen();
};