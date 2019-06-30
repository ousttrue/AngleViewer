#pragma once
#include "objectbase.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include "material.h"
#include <simplegltf/simplegltf.h>

namespace agv
{
namespace scene
{

class Mesh : ObjectBase
{
public:
    Mesh(const std::string &name) : ObjectBase(name)
    {
    }

    simplegltf::GltfTopologyType Topology = simplegltf::GltfTopologyType::TRIANGLES;

    std::shared_ptr<scene::Material> Material;

    std::unordered_map<std::string, simplegltf::View> VertexAttributes;
    void AddVertexAttribute(const std::string &semantic, simplegltf::View view)
    {
        VertexAttributes.insert(std::make_pair(semantic, view));
    }
    size_t GetVertexCount() const
    {
        auto found = VertexAttributes.find("POSITION");
        if (found == VertexAttributes.end())
        {
            return 0;
        }
        return found->second.get_count();
    }

    simplegltf::View Indices;

    static std::shared_ptr<Mesh> CreateGrid(const std::shared_ptr<scene::Material> &material,
                                            float size, int count);
    static std::shared_ptr<Mesh> CreateAxis(const std::shared_ptr<scene::Material> &material,
                                            float size);
    static std::shared_ptr<Mesh> CreateSampleTriangle(const std::shared_ptr<scene::Material> &material,
                                                      float size);
};
} // namespace scene
} // namespace agv
