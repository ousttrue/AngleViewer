#pragma once
#include "material.h"


namespace agv
{
namespace renderer
{

struct ShaderSource
{
    std::string vs;
    std::string fs;

    ShaderSource(const std::string &vs_, const std::string &fs_)
        : vs(vs_), fs(fs_)
    {
    }
};

///
/// singleton
///
class ShaderSourceManager
{
    std::unordered_map<scene::ShaderType, std::shared_ptr<ShaderSource>> m_sourceMap;

    ShaderSourceManager();

public:
    std::shared_ptr<ShaderSource> GetSource(scene::ShaderType shaderType);

    void SetSource(scene::ShaderType shaderType, const std::string &vs, const std::string &fs)
    {
        auto found = m_sourceMap.find(shaderType);
        if (found != m_sourceMap.end())
        {
            found->second->vs = fs;
            found->second->fs = fs;
        }
        else
        {
            m_sourceMap.insert(std::make_pair(shaderType, std::make_shared<ShaderSource>(vs, fs)));
        }
    }

public:
    static ShaderSourceManager Instance;
};

} // namespace renderer
} // namespace agv