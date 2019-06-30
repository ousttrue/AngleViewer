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

class Texture;
class Material : public ObjectBase
{
public:
    ShaderType ShaderType = ShaderType::gizmo;

    std::shared_ptr<Texture> ColorTexture;

    Material(const std::string &name) : ObjectBase(name)
    {
    }

    static std::shared_ptr<Material> Load(const simplegltf::Storage &storage,
                                          const simplegltf::GltfMaterial &material,
                                          const std::vector<std::shared_ptr<Texture>> &textures);
};

} // namespace scene
} // namespace agv
