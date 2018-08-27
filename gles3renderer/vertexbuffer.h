#pragma once
#include <stdint.h>
#include <vector>
#include <memory>


class VertexBuffer
{
	uint32_t m_vbo=0;

public:
	VertexBuffer();
    ~VertexBuffer();
	void Bind();
	void BufferData(const std::vector<float> &vertices);
};


class VertexArray
{
	uint32_t m_vao = 0;

	std::vector<std::shared_ptr<VertexBuffer>> m_attributes;

public:
	VertexArray();
	~VertexArray();
	static std::shared_ptr<VertexArray> Create(const std::vector<float> &vertices);
	void Bind();
	void AddAttribute(const std::shared_ptr<VertexBuffer> &vbo);
	void Draw();
};
