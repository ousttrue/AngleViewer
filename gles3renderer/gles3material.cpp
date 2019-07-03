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
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);
    glDisable(GL_CULL_FACE);
}

} // namespace renderer
} // namespace agv