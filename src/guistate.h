#pragma
#include <string>

namespace agv::scene
{
class Scene;

} // namespace agv::scene

struct GuiState
{
    bool loggerOpen = true;
    std::string logger;

    void Update(class agv::scene::Scene *scene);
};
