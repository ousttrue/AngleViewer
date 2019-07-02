#include "gles3texture.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#include <exception>

namespace agv
{
namespace renderer
{

GLES3Texture::GLES3Texture()
{
    glGenTextures(1, &m_texture);
}

GLES3Texture::~GLES3Texture()
{
    if (m_texture)
    {
        glDeleteTextures(1, &m_texture);
        m_texture = 0;
    }
}

void GLES3Texture::Bind(int slot)
{
    // glActiveTexture(GL_TEXTURE0 + slot);

    glBindTexture(GL_TEXTURE_2D, m_texture);
}

void GLES3Texture::Unbind(int slot)
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLES3Texture::SetImage(int width, int height, int channels, const std::byte *data)
{
    Bind(0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    GLint format = 0;
    switch (channels)
    {
    case 3:
        format = GL_RGB;
        break;

    case 4:
        format = GL_RGBA;
        break;

    default:
        throw std::exception("invalid channels");
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0,
                 format, GL_UNSIGNED_BYTE, static_cast<const void *>(data));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    Unbind(0);
}

} // namespace renderer
} // namespace agv