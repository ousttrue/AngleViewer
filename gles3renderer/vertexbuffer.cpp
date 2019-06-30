#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#include <assert.h>

namespace agv
{
namespace renderer
{
///
/// VertexBuffer
///
VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_vbo);
    assert(m_vbo);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_vbo);
    m_vbo = 0;
}

void VertexBuffer::Bind()
{
    if (m_isIndex)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    }
}

void VertexBuffer::Unbind()
{
    if (m_isIndex)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void VertexBuffer::BufferData(bool isIndex, const std::byte *values, size_t byteSize, int componentCount)
{
    m_isIndex = isIndex;
    m_componentCount = componentCount;

    Bind();
    {
        if (m_isIndex)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, values, GL_STATIC_DRAW);
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, byteSize, values, GL_STATIC_DRAW);
        }
        Unbind();
    }
}

///
/// VertexBufferGroup
///
VertexBufferGroup::VertexBufferGroup(int vertexCount, simplegltf::GltfTopologyType topology)
    : m_vertexCount(vertexCount), m_topology(static_cast<uint32_t>(topology))
{
}

void VertexBufferGroup::AddAttribute(const std::string &semantic, const simplegltf::View &view)
{
    auto vbo = std::make_shared<VertexBuffer>();
    vbo->BufferData(false, view.data, view.size, simplegltf::get_component_count(view.valuetype));
    m_attributes.insert(std::make_pair(semantic, vbo));
    vbo->Unbind();
}

void VertexBufferGroup::SetIndex(const simplegltf::View &view)
{
    auto vbo = std::make_shared<VertexBuffer>();
    vbo->BufferData(true, view.data, view.size, 1);
    m_indices = vbo;
    vbo->Unbind();
    m_indexCount = view.get_count();
    switch (view.valuetype)
    {
    case simplegltf::ValueType::UInt16:
        m_indexType = GL_UNSIGNED_SHORT;
        break;
    case simplegltf::ValueType::UInt32:
        m_indexType = GL_UNSIGNED_INT;
        break;

    default:
        throw std::exception("not implemented");
    }
}

void VertexBufferGroup::Draw(int offset, int count)
{
    if (m_indices)
    {
        uint64_t offset64 = offset;
        glDrawElements(m_topology, count, m_indexType, reinterpret_cast<const void*>(offset64));
    }
    else
    {
        glDrawArrays(m_topology, offset, count);
    }
}

///
/// VertexArray
///
VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_vao);
    assert(m_vao);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vao);
    m_vao = 0;
}

void VertexArray::Bind()
{
    glBindVertexArray(m_vao);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::BindSlot(int slot, const std::shared_ptr<VertexBuffer> &vbo)
{
    glEnableVertexAttribArray(slot);
    glVertexAttribPointer(slot, vbo->GetComponentCount(), GL_FLOAT, GL_FALSE, 0, nullptr);
}

void VertexArray::UnbindSlot(int slot)
{
    glDisableVertexAttribArray(slot);
}

} // namespace renderer
} // namespace agv
