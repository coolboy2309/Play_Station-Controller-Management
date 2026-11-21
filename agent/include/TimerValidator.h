#pragma once

#include <string>

class TimerValidator
{
public:

    static bool IsValid(const std::string& timer);

    static bool IsSequential(
        const std::string& previous,
        const std::string& current);

    static bool IsNewMatch(
        const std::string& previous,
        const std::string& current);
};