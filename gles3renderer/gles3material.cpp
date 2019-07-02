#include "gles3material.h"
#include "gles3texture.h"
#include "gles3shader.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>

namespace agv
{
namespace renderer
{

void GLES3Material::Set()
{
    if (Texture)
    {
        auto slot = 0;
        Texture->Bind(slot);
        Shader->SetUniformValue("Color", slot);
    }
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);
}

} // namespace renderer
} // namespace agv