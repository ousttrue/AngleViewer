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

void VertexBuffer::BufferData(bool isIndex, const std::byte *values, size_t byteSize)
{
    m_isIndex = isIndex;

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
/// VertexArray
///
VertexArray::VertexArray(int vertexCount, simplegltf::GltfTopologyType topology)
    : m_vertexCount(vertexCount), m_topology(static_cast<uint32_t>(topology))
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

void VertexArray::AddAttribute(const std::string &semantic, const simplegltf::View &view)
{
    auto vbo = std::make_shared<VertexBuffer>();
    vbo->BufferData(false, view.data, view.size);

    auto attribute = static_cast<GLuint>(m_attributes.size());
    m_attributes.insert(std::make_pair(semantic, vbo));

    Bind();
    {
        vbo->Bind();
        glEnableVertexAttribArray(attribute);
        glVertexAttribPointer(attribute, simplegltf::get_component_count(view.valuetype), GL_FLOAT, GL_FALSE, 0, nullptr);
        Unbind();
    }

    glDisableVertexAttribArray(attribute);
    vbo->Unbind();
}

void VertexArray::SetIndex(const simplegltf::View &view)
{
    auto vbo = std::make_shared<VertexBuffer>();
    vbo->BufferData(true, view.data, view.size);

    m_indices = vbo;
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

    Bind();
    {
        vbo->Bind();
        Unbind();
    }
    vbo->Unbind();
    }

    void VertexArray::Draw()
    {
        Bind();

        if (m_indices)
        {
            glDrawElements(m_topology, m_indexCount, m_indexType, 0);
        }
        else
        {
            glDrawArrays(m_topology, 0, m_vertexCount);
        }
    }
} // namespace renderer
} // namespace renderer
