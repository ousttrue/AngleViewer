#include "shadersourcemanager.h"
#include <plog/Log.h>

namespace agv
{
namespace renderer
{

ShaderSourceManager ShaderSourceManager::Instance;

std::shared_ptr<ShaderSource> ShaderSourceManager::GetSource(scene::ShaderType shaderType)
{
    auto found = m_sourceMap.find(shaderType);
    if (found == m_sourceMap.end())
    {
        LOGE << "shader not found";
        return nullptr;
    }
    return found->second;
}

} // namespace renderer
} // namespace agv