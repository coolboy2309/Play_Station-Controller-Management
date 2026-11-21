
#include "../include/FullTimeDetector.h"
#include "../include/Logger.h"

#include <opencv2/imgproc.hpp>
#include <algorithm>
#include <cctype>

FullTimeDetector::FullTimeDetector()
{
    if (tess.Init(
            "C:/Program Files/Tesseract-OCR/tessdata",
            "eng",
            tesseract::OEM_LSTM_ONLY) != 0)
    {
        Logger::Error(
            "FullTimeDetector: Tesseract initialization failed.");
        return;
    }

    // The crop contains one short line of text.
    tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    tess.SetVariable(
        "tessedit_char_whitelist",
        "FULLTIMEfulltime");

    tess.SetVariable("load_system_dawg", "0");
    tess.SetVariable("load_freq_dawg", "0");

    initialized = true;

    Logger::Info("FullTimeDetector initialized.");
}

FullTimeDetector::~FullTimeDetector()
{
    if (initialized)
        tess.End();
}

std::string FullTimeDetector::ReadText(const cv::Mat &image)
{
    if (!initialized || image.empty())
        return "";

    cv::Mat gray;
    cv::Mat enlarged;
    cv::Mat processed;

    if (image.channels() == 3)
    {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }
    else if (image.channels() == 4)
    {
        cv::cvtColor(image, gray, cv::COLOR_BGRA2GRAY);
    }
    else
    {
        gray = image.clone();
    }

    // Enlarge the FULL-TIME text for better OCR.
    cv::resize(
        gray,
        enlarged,
        cv::Size(),
        4.0,
        4.0,
        cv::INTER_CUBIC);

    // Separate bright white text from the dark purple background.
    cv::threshold(
        enlarged,
        processed,
        0,
        255,
        cv::THRESH_BINARY | cv::THRESH_OTSU);

    tess.SetImage(
        processed.data,
        processed.cols,
        processed.rows,
        1,
        static_cast<int>(processed.step));

    if (tess.Recognize(nullptr) != 0)
    {
        Logger::Error("FullTimeDetector OCR recognition failed.");
        tess.Clear();
        return "";
    }

    char *result = tess.GetUTF8Text();

    std::string text = result ? result : "";

    delete[] result;

    tess.Clear();

    Logger::Info("FULL TIME OCR RAW: [" + text + "]");

    return text;
}

bool FullTimeDetector::IsFullTimeText(const std::string &text)
{
    std::string normalized;

    for (unsigned char character : text)
    {
        if (std::isalpha(character))
        {
            normalized += static_cast<char>(
                std::toupper(character));
        }
    }

    return normalized.find("FULLTIME") != std::string::npos;
}

bool FullTimeDetector::Detect(const cv::Mat &screenImage)
{
    if (!initialized || screenImage.empty())
    {
        confirmationCount = 0;
        return false;
    }

    const int screenWidth = screenImage.cols;
    const int screenHeight = screenImage.rows;

    // Target the bottom-center scoreboard banner.
    const int x = static_cast<int>(screenWidth * 0.43);
    const int y = static_cast<int>(screenHeight * 0.87);
    const int width = static_cast<int>(screenWidth * 0.14);
    const int height = static_cast<int>(screenHeight * 0.10);

    cv::Rect roi(x, y, width, height);

    // Safety check before cropping.
    roi &= cv::Rect(
        0, 0, screenWidth, screenHeight);

    if (roi.empty())
    {
        confirmationCount = 0;
        return false;
    }

    cv::Mat fullTimeRegion = screenImage(roi).clone();

    // Focus on the actual FULL-TIME letters.
    cv::Rect textRoi(
        static_cast<int>(fullTimeRegion.cols * 0.20),
        static_cast<int>(fullTimeRegion.rows * 0.38),
        static_cast<int>(fullTimeRegion.cols * 0.60),
        static_cast<int>(fullTimeRegion.rows * 0.34));

    cv::Mat textRegion = fullTimeRegion(textRoi).clone();

    // Save temporarily for inspection.
    cv::imwrite("full_time_text.png", textRegion);

    std::string text = ReadText(textRegion);

    if (IsFullTimeText(text))
    {
        confirmationCount++;

        Logger::Info(
            "FULL TIME candidate: " +
            std::to_string(confirmationCount) + "/3");

        if (confirmationCount >= REQUIRED_CONFIRMATIONS)
        {
            confirmationCount = 0;

            Logger::Info("FULL TIME CONFIRMED");

            return true;
        }
    }
    else
    {
        confirmationCount = 0;
    }

    return false;
}
