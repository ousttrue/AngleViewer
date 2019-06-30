#pragma once
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
class Mesh
{
public:
    enum Topology
    {
        Triangles,
        Lines,
    };

private:
    renderer::Material m_material;
    Topology m_topology = Topology::Triangles;

public:
    std::unordered_map<std::string, simplegltf::View> VertexAttributes;
    void AddVertexAttribute(const std::string &semantic, simplegltf::View view)
    {
        VertexAttributes.insert(std::make_pair(semantic, view));
    }
	size_t GetVertexCount()const { 
		auto found = VertexAttributes.find("POSITION");
		if(found==VertexAttributes.end()){
			return 0;
		}
		return found->second.get_count();
	}

    simplegltf::View Indices;

    static std::shared_ptr<Mesh> CreateGrid(const renderer::Material &material,
                                            float size, int count);
    static std::shared_ptr<Mesh> CreateAxis(const renderer::Material &material,
                                            float size);
    static std::shared_ptr<Mesh> CreateSampleTriangle(const renderer::Material &material,
                                                      float size);

    static std::shared_ptr<Mesh> CreateFromGltf(const renderer::Material &material,
                                                simplegltf::Storage &gltf, int meshIndex);

    const renderer::Material &GetMaterial() const { return m_material; }
    Topology GetTopology() const { return m_topology; }
};
} // namespace scene
} // namespace agv
