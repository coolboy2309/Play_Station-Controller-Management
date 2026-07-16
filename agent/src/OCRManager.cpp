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

    tess.SetPageSegMode(tesseract::PSM_SINGLE_WORD);

    tess.SetVariable(
        "tessedit_char_whitelist",
        "0123456789:");

    Pix *image = pixRead(imagePath.c_str());

    if (!image)
    {
        tess.End();
        return "Image Load Failed";
    }

    tess.SetPageSegMode(tesseract::PSM_SINGLE_WORD);

    tess.SetImage(image);

    tess.SetSourceResolution(300);

    tess.Recognize(0);

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