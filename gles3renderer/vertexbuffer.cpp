#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <assert.h>


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
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void VertexBuffer::BufferData(const std::vector<float> &vertices)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}


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

std::shared_ptr<VertexArray> VertexArray::Create(const std::vector<float> &vertices)
{
	auto vao = std::make_shared<VertexArray>();

	auto vbo = std::make_shared<VertexBuffer>();
	vbo->BufferData(vertices);
	vao->AddAttribute(vbo);

	return vao;
}

void VertexArray::Bind()
{
	glBindVertexArray(m_vao);
}

void VertexArray::AddAttribute(const std::shared_ptr<VertexBuffer> &vbo)
{
	Bind();
	auto attribute = m_attributes.size();
	m_attributes.push_back(vbo);

	glEnableVertexAttribArray(attribute);
	vbo->Bind();
	glVertexAttribPointer(attribute, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void VertexArray::Draw()
{
	Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
