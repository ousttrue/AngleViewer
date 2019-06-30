#pragma once
#include "objectbase.h"

namespace agv
{
namespace scene
{

class Material : public ObjectBase
{
public:
    Material(const std::string &name) : ObjectBase(name)
    {
    }

    std::string vs;
    std::string fs;
};

} // namespace renderer
} // namespace agv
