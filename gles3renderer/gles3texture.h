#pragma once
#include <cstdint>
#include <cstddef>

namespace agv
{
namespace renderer
{

class GLES3Texture
{
    uint32_t m_texture = 0;

public:
    GLES3Texture();
    ~GLES3Texture();
    void Bind(int slot);
    static void Unbind(int slot);
    void GLES3Texture::SetImage(int width, int height, int channels, const std::byte *data);
};

} // namespace renderer
} // namespace agv
