#include "../include/OCRManager.h"
#include "../include/Logger.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <algorithm>

std::string OCRManager::ReadSingleImage(
    const std::string &imagePath,
    int &confidence)
{
    tesseract::TessBaseAPI tess;

    if (tess.Init(
            "C:/Program Files/Tesseract-OCR/tessdata",
            "eng"))
    {
        return "";
    }

    tess.SetPageSegMode(
        tesseract::PSM_SINGLE_LINE);

    tess.SetVariable(
        "tessedit_char_whitelist",
        "0123456789:");

    tess.SetVariable(
        "load_system_dawg",
        "0");

    tess.SetVariable(
        "load_freq_dawg",
        "0");

    Pix *image =
        pixRead(imagePath.c_str());

    if (!image)
    {
        tess.End();
        return "";
    }

    Logger::Info(
        "Pix Width = " +
        std::to_string(
            pixGetWidth(image)));

    Logger::Info(
        "Pix Height = " +
        std::to_string(
            pixGetHeight(image)));

    Logger::Info(
        "Pix Depth = " +
        std::to_string(
            pixGetDepth(image)));

    tess.SetImage(image);

    tess.Recognize(0);

    confidence =
        tess.MeanTextConf();

    Logger::Info(
        "Mean confidence = " +
        std::to_string(confidence));

    char *out =
        tess.GetUTF8Text();

    std::string result;

    if (out)
    {
        result = out;

        result.erase(
            std::remove(
                result.begin(),
                result.end(),
                '\n'),
            result.end());

        result.erase(
            std::remove(
                result.begin(),
                result.end(),
                '\r'),
            result.end());

        result.erase(
            std::remove(
                result.begin(),
                result.end(),
                ' '),
            result.end());

        if (result.length() >= 5)
            result = result.substr(0, 5);

        if (result.size() != 5 ||
            result[2] != ':')
        {
            result.clear();
        }
    }

    delete[] out;

    pixDestroy(&image);

    tess.End();

    Logger::Info(
        "OCR Raw = [" +
        result +
        "]");

    return result;
}

std::string OCRManager::ReadTimer()
{
    int grayConf = 0;
    int binaryConf = 0;

    std::string gray =
        ReadSingleImage(
            "timer_gray.jpg",
            grayConf);

    std::string binary =
        ReadSingleImage(
            "timer_binary.jpg",
            binaryConf);

    Logger::Info(
        "Gray Confidence : " +
        std::to_string(grayConf));

    Logger::Info(
        "Binary Confidence : " +
        std::to_string(binaryConf));

    std::string timer;

    if (!binary.empty())
    {
        Logger::Info("Using Binary OCR");
        timer = binary;
    }
    else
    {
        Logger::Info("Using Gray OCR");
        timer = gray;
    }

    timer = stabilizer.Update(timer);

    Logger::Info("Stable Timer : " + timer);

    return timer;
}