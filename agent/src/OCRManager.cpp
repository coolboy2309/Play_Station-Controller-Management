#include "../include/OCRManager.h"
#include "../include/Logger.h"
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <algorithm>

std::string OCRManager::ReadTimer(const std::string &imagePath)
{
    tesseract::TessBaseAPI tess;

    if (tess.Init("C:/Program Files/Tesseract-OCR/tessdata", "eng"))
    {
        return "Tesseract Init Failed";
    }

    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);

    tess.SetVariable(
        "tessedit_char_whitelist",
        "0123456789:");

    Pix *image = pixRead(imagePath.c_str());
    Logger::Info("Pix Width = " + std::to_string(pixGetWidth(image)));
    Logger::Info("Pix Height = " + std::to_string(pixGetHeight(image)));
    Logger::Info("Pix Depth = " + std::to_string(pixGetDepth(image)));
    Logger::Info(
        "Image Width = " + std::to_string(pixGetWidth(image)));

    Logger::Info(
        "Image Height = " + std::to_string(pixGetHeight(image)));

    Logger::Info(
        "Image Depth = " + std::to_string(pixGetDepth(image)));

    if (!image)
    {
        tess.End();
        return "Image Load Failed";
    }

    tess.SetImage(image);

    tess.SetSourceResolution(300);

    tess.Recognize(0);

    Logger::Info(
        "Mean confidence = " +
        std::to_string(tess.MeanTextConf()));

    char *out = tess.GetUTF8Text();

    std::string result;

    if (out)
    {
        result = out;

        result.erase(
            std::remove(result.begin(), result.end(), '\n'),
            result.end());

        result.erase(
            std::remove(result.begin(), result.end(), '\r'),
            result.end());

        result.erase(
            std::remove(result.begin(), result.end(), ' '),
            result.end());
    }

    delete[] out;

    pixDestroy(&image);

    tess.End();

    Logger::Info("OCR Raw = [" + result + "]");

    return result;
}