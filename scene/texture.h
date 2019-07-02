#pragma once
#include "objectbase.h"

namespace agv
{
namespace scene
{

class Texture : public ObjectBase
{
    std::string m_mime;
    simplegltf::View m_bytes;

public:
    Texture(const std::string &name) : ObjectBase(name)
    {
    }

    void SetBytes(const std::string &mime, const simplegltf::View &view)
    {
        m_mime = mime;
        m_bytes = view;
    }

    static std::shared_ptr<Texture> Load(const simplegltf::Storage &storage, const simplegltf::GltfTexture &gltfTexture);
};

} // namespace scene
} // namespace agv