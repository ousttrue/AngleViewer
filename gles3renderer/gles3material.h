#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace agv
{
namespace renderer
{

class GLES3Shader;
class GLES3Texture;
class GLES3Material
{
public:
    std::shared_ptr<GLES3Shader> Shader;

    glm::vec4 Color = glm::vec4(1, 1, 1, 1);

    std::shared_ptr<GLES3Texture> Texture;

    void Set();
};

} // namespace renderer
} // namespace agv