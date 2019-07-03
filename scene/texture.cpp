#include "texture.h"

namespace agv
{
namespace scene
{

std::shared_ptr<Texture> Texture::Load(const simplegltf::Storage &storage, const simplegltf::GltfTexture &gltfTexture)
{
    auto &gltf = storage.gltf;
    auto &image = gltf.images[gltfTexture.source];

    auto name = gltfTexture.name;
    if(name.empty()){
        name = image.name;
    }

    auto texture = std::make_shared<Texture>(name);

    texture->SetBytes(image.mimeType, storage.get_from_image(image));

    return texture;
}

} // namespace scene
} // namespace agv