#pragma once
#include <string>

class CaptureManager
{
public:

    void Capture(const std::string& competition);

    std::string GetCompetition() const;

private:

    std::string detectedCompetition;
};