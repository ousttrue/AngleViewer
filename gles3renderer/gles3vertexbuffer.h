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
class GLES3VertexBuffer
{
    bool m_isIndex = false;
    uint32_t m_vbo = 0;
    int m_componentCount = 0;

public:
    GLES3VertexBuffer();
    ~GLES3VertexBuffer();
    void Bind();
    void Unbind();
    void BufferData(bool isIndex, const std::byte *values, size_t byteSize, int componentCount);
    int GetComponentCount() const { return m_componentCount; }
};

///
/// scene Mesh から作られるVBOをグループ化する
///
class GLES3VertexBufferGroup
{
public:
    std::unordered_map<std::string, std::shared_ptr<GLES3VertexBuffer>> m_attributes;
    std::shared_ptr<GLES3VertexBuffer> m_indices;

private:
    uint32_t m_topology;
    int m_vertexCount;

    int m_indexCount;
    uint32_t m_indexType;

public:
    GLES3VertexBufferGroup(int vertexCount, simplegltf::GltfTopologyType topology);
    GLES3VertexBufferGroup(int vertexCount) : GLES3VertexBufferGroup(vertexCount, simplegltf::GltfTopologyType::TRIANGLES)
    {
    }
    void AddAttribute(const std::string &semantic, const simplegltf::View &view);
    void SetIndex(const simplegltf::View &view);
    void Draw(int offset, int count);
};

///
/// GLES3VertexBufferGroup と Material からVAOを作成する(Shaderの頂点Attributeの並びが違うかもしれない)
///
class GLES3VertexArray
{
    uint32_t m_vao = 0;

public:
    GLES3VertexArray();
    ~GLES3VertexArray();
    void BindSlot(int slot, const std::shared_ptr<GLES3VertexBuffer> &vbo);
	void UnbindSlot(int slot);
    void Bind();
    static void Unbind();
};
} // namespace renderer
} // namespace agv
