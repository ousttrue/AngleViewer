#include "texture.h"

namespace agv
{
namespace scene
{

std::shared_ptr<Texture> Texture::Load(const simplegltf::Storage &storage, const simplegltf::GltfTexture &gltfTexture)
{
    auto texture = std::make_shared<Texture>(gltfTexture.name);
    auto &gltf = storage.gltf;

    auto &image = gltf.images[gltfTexture.source];

    if (image.uri.empty())
    {
        // from buffer
        auto &bufferView = gltf.bufferViews[image.bufferView];
        texture->SetBytes(image.mimeType, storage.get_from_bufferView(bufferView));
    }
    else
    {
        // external file
        throw std::exception("not implemented");
    }

    return texture;
}

} // namespace scene
} // namespace agv