#include "../include/OCRManager.h"

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

std::string OCRManager::ReadTimer(const std::string &imagePath)
{
    tesseract::TessBaseAPI tess;

    tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    tess.SetVariable(
        "tessedit_char_whitelist",
        "0123456789:");

    if (tess.Init("C:/Program Files/Tesseract-OCR/tessdata", "eng"))
    {
        return "Tesseract Init Failed";
    }

    Pix *image = pixRead(imagePath.c_str());

    if (!image)
    {
        tess.End();
        return "Image Load Failed";
    }

    tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

    tess.SetImage(image);

    char *out = tess.GetUTF8Text();

    std::string result;

    if (out)
        result = out;

    delete[] out;

    pixDestroy(&image);

    tess.End();

    return result;
}