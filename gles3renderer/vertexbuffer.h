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

	uint32_t m_topology;
	int m_vertexCount;

public:
	VertexArray(uint32_t topology, int vertexCount);
	~VertexArray();
	static std::shared_ptr<VertexArray> Create(uint32_t topology, const std::vector<float> &vertices, const std::vector<float> &colors);
	static std::shared_ptr<VertexArray> CreateTriangles(const std::vector<float> &vertices, const std::vector<float> &colors);
	static std::shared_ptr<VertexArray> CreateLines(const std::vector<float> &vertices, const std::vector<float> &colors);

	void Bind();
	void AddAttribute(const std::shared_ptr<VertexBuffer> &vbo, int components);
	void Draw();
};
