#include "material.h"
#include "texture.h"

namespace agv
{
namespace scene
{

std::shared_ptr<Material> Material::Load(const simplegltf::Storage &storage,
                                         const simplegltf::GltfMaterial &gltfMaterial,
                                         const std::vector<std::shared_ptr<Texture>> &textures)
{
    auto &gltf = storage.gltf;
    auto material = std::make_shared<Material>(gltfMaterial.name);

    material->ShaderType = ShaderType::unlit;

    if (gltfMaterial.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        material->ColorTexture = textures[gltfMaterial.pbrMetallicRoughness.baseColorTexture.index];
    }

    return material;
}

} // namespace scene
} // namespace agv
