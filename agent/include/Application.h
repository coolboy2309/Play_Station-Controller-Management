#pragma once

#include "Config.h"

class Application
{
public:

    bool Initialize();

    void Run();

private:

    Config config;
};