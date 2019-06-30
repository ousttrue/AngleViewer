#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <simplegltf/simplegltf.h>

namespace agv
{
namespace renderer
{
class VertexBuffer
{
    bool m_isIndex = false;
    uint32_t m_vbo = 0;
    int m_componentCount = 0;

public:
    VertexBuffer();
    ~VertexBuffer();
    void Bind();
    void Unbind();
    void BufferData(bool isIndex, const std::byte *values, size_t byteSize, int componentCount);
    int GetComponentCount() const { return m_componentCount; }
};

///
/// scene Mesh から作られるVBOをグループ化する
///
class VertexBufferGroup
{
public:
    std::unordered_map<std::string, std::shared_ptr<VertexBuffer>> m_attributes;
    std::shared_ptr<VertexBuffer> m_indices;

private:
    uint32_t m_topology;
    int m_vertexCount;

    int m_indexCount;
    uint32_t m_indexType;

public:
    VertexBufferGroup(int vertexCount, simplegltf::GltfTopologyType topology);
    VertexBufferGroup(int vertexCount) : VertexBufferGroup(vertexCount, simplegltf::GltfTopologyType::TRIANGLES)
    {
    }
    void AddAttribute(const std::string &semantic, const simplegltf::View &view);
    void SetIndex(const simplegltf::View &view);
    void Draw(int offset, int count);
};

///
/// VertexBufferGroup と Material からVAOを作成する(Shaderの頂点Attributeの並びが違うかもしれない)
///
class VertexArray
{
    uint32_t m_vao = 0;

public:
    VertexArray();
    ~VertexArray();
    void BindSlot(int slot, const std::shared_ptr<VertexBuffer> &vbo);
	void UnbindSlot(int slot);
    void Bind();
    void Unbind();
};
} // namespace renderer
} // namespace agv
