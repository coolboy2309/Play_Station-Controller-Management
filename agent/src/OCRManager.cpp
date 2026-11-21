
#include "../include/OCRManager.h"
#include "../include/Logger.h"

#include <algorithm>
#include <cctype>

OCRManager::OCRManager()
{
    if (tess.Init(
        "C:/Program Files/Tesseract-OCR/tessdata",
        "eng"))
    {
        Logger::Error("Tesseract initialization failed");
        return;
    }

    initialized = true;

    tess.SetPageSegMode(
        tesseract::PSM_SINGLE_LINE
    );

    tess.SetVariable(
        "tessedit_char_whitelist",
        "0123456789:"
    );

    tess.SetVariable(
        "load_system_dawg",
        "0"
    );

    tess.SetVariable(
        "load_freq_dawg",
        "0"
    );

    Logger::Info("Tesseract Initialized");
}


OCRManager::~OCRManager()
{
    if (initialized)
    {
        tess.End();
    }
}


std::string OCRManager::ReadSingleImage(
    const cv::Mat& image,
    int& confidence)
{
    confidence = 0;

    if (!initialized || image.empty())
    {
        return "";
    }

    if (image.type() != CV_8UC1)
    {
        Logger::Error("OCR image must be 8-bit grayscale");
        return "";
    }

    Logger::Info(
        "OCR Image Width = " + std::to_string(image.cols)
    );

    Logger::Info(
        "OCR Image Height = " + std::to_string(image.rows)
    );

    // Pass the OpenCV image directly to Tesseract.
    tess.SetImage(
        image.data,
        image.cols,
        image.rows,
        1,
        static_cast<int>(image.step)
    );

    if (tess.Recognize(nullptr) != 0)
    {
        Logger::Error("Tesseract recognition failed");
        tess.Clear();
        return "";
    }

    confidence = tess.MeanTextConf();

    Logger::Info(
        "Mean confidence = " + std::to_string(confidence)
    );

    char* out = tess.GetUTF8Text();

    std::string result;

    if (out)
    {
        result = out;

        delete[] out;
    }

    // Remove whitespace and line endings.
    result.erase(
        std::remove_if(
            result.begin(),
            result.end(),
            [](unsigned char c)
            {
                return std::isspace(c);
            }
        ),
        result.end()
    );

    // A timer must have the format MM:SS.
    if (result.size() != 5 || result[2] != ':')
    {
        result.clear();
    }

    Logger::Info("OCR Raw = [" + result + "]");

    tess.Clear();

    return result;
}


std::string OCRManager::ReadTimer(
    const cv::Mat& grayImage,
    const cv::Mat& binaryImage)
{
    int grayConf = 0;
    int binaryConf = 0;

    std::string gray = ReadSingleImage(
        grayImage,
        grayConf
    );

    std::string binary = ReadSingleImage(
        binaryImage,
        binaryConf
    );

    Logger::Info(
        "Gray Confidence : " + std::to_string(grayConf)
    );

    Logger::Info(
        "Binary Confidence : " + std::to_string(binaryConf)
    );

    std::string timer;

    // Prefer the result with higher confidence when both are valid.
    if (!gray.empty() && !binary.empty())
    {
        if (binaryConf > grayConf)
        {
            Logger::Info("Using Binary OCR");
            timer = binary;
        }
        else
        {
            Logger::Info("Using Gray OCR");
            timer = gray;
        }
    }
    else if (!binary.empty())
    {
        Logger::Info("Using Binary OCR");
        timer = binary;
    }
    else if (!gray.empty())
    {
        Logger::Info("Using Gray OCR");
        timer = gray;
    }

    timer = stabilizer.Update(timer);

    Logger::Info("Stable Timer : " + timer);

    return timer;
}
