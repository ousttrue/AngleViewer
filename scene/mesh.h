#pragma once
#include "objectbase.h"

namespace agv
{
namespace scene
{

class Material;
class Submesh : public ObjectBase
{
    std::shared_ptr<Material> m_material;
    int m_drawCount;

public:
    Submesh(const std::string &name,
            int drawCount,
            const std::shared_ptr<Material> &material)
        : ObjectBase(name), m_drawCount(drawCount), m_material(material)
    {
    }

    const std::shared_ptr<Material> &GetMaterial() const { return m_material; }
    int GetDrawCount() const { return m_drawCount; }
};

class Mesh : public ObjectBase
{
public:
    Mesh(const std::string &name) : ObjectBase(name)
    {
    }

    simplegltf::GltfTopologyType Topology = simplegltf::GltfTopologyType::TRIANGLES;

    std::vector<std::shared_ptr<Submesh>> Submeshes;
    void WholeSubmesh(const std::shared_ptr<Material> &material);

    std::unordered_map<std::string, simplegltf::View> VertexAttributes;
    void AddVertexAttribute(const std::string &semantic, simplegltf::View view)
    {
        VertexAttributes.insert(std::make_pair(semantic, view));
    }
    int GetVertexCount() const
    {
        auto found = VertexAttributes.find("POSITION");
        if (found == VertexAttributes.end())
        {
            return 0;
        }
        return static_cast<int>(found->second.get_count());
    }

    simplegltf::View Indices;

    static std::shared_ptr<Mesh> CreateGrid(float size, int count);
    static std::shared_ptr<Mesh> CreateAxis(float size);
    static std::shared_ptr<Mesh> CreateSampleTriangle(float size);
};

class MeshGroup : public ObjectBase
{
public:
    std::vector<std::shared_ptr<Mesh>> Meshes;

    MeshGroup(const std::string &name) : ObjectBase(name)
    {
    }

    static std::shared_ptr<MeshGroup> Load(const simplegltf::Storage &storage,
                                           const simplegltf::GltfMesh &gltfMesh,
                                           const std::vector<std::shared_ptr<Material>> &materials);

    static std::shared_ptr<MeshGroup> Create(const std::shared_ptr<Mesh> &mesh);
};

} // namespace scene
} // namespace agv
