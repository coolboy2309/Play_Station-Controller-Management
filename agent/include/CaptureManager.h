
#pragma once

#include <string>
#include <opencv2/opencv.hpp>

class CaptureManager
{
public:
    bool Capture(
        const std::string& imagePath,
        const std::string& competition,
        cv::Mat& grayImage,
        cv::Mat& binaryImage
    );

    std::string GetCompetition() const;

    cv::Mat GetLastFrame() const;

private:
    std::string detectedCompetition;

    cv::Mat lastFrame;

    cv::Mat CaptureScreen();
};
