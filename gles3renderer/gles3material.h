#pragma once
#include <memory>

namespace agv
{
namespace renderer
{

class GLES3Shader;
class GLES3Material
{
public:
    std::shared_ptr<GLES3Shader> Shader;
};

} // namespace renderer
} // namespace agv