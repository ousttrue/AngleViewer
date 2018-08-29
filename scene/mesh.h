#pragma once
#include <string>
#include <vector>


class Mesh
{
	std::string m_vs;
	std::string m_fs;
	std::vector<float> m_vertices;

public:
	Mesh(const std::string &vs,
		const std::string &fs,
		const std::vector<float> &vertices)
		: m_vs(vs), m_fs(fs), m_vertices(vertices)
	{}

	std::string GetVertexShader()const { return m_vs; }
	std::string GetFragmentShader()const { return m_fs; }
	std::vector<float> GetVertices()const { return m_vertices; }
};
