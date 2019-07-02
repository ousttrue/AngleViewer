#pragma
#include <string>

struct GuiState
{
    bool loggerOpen = true;
    std::string logger;

    void Update();
};
