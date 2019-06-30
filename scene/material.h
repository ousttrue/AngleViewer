#pragma once
#include "objectbase.h"

namespace agv
{
namespace scene
{

enum class ShaderType
{
    gizmo,
    unlit,
    pbr,
};

class Material : public ObjectBase
{
public:
    ShaderType ShaderType = ShaderType::gizmo; 

    Material(const std::string &name) : ObjectBase(name)
    {
    }
};

} // namespace renderer
} // namespace agv
