#pragma once

#include "TimerStabilizer.h"
#include <string>

class OCRManager
{
    public:

    std::string ReadTimer();


    private:

    std::string ReadSingleImage(
        const std::string& imagePath,
        int& confidence
    );

    private:

    TimerStabilizer stabilizer;

};