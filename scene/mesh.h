#pragma once
#include <string>
#include <vector>


class Mesh
{
public:
	enum Topology
	{
		Triangles,
		Lines,
	};

private:

	std::string m_vs;
	std::string m_fs;
	Topology m_topology;
	std::vector<float> m_vertices;
	std::vector<float> m_colors;

public:
	Mesh(const std::string &vs,
		const std::string &fs,
		Topology topology,
		const std::vector<float> &vertices,
		const std::vector<float> &colors
		)
		: m_vs(vs), m_fs(fs), m_topology(topology),
		m_vertices(vertices), m_colors(colors)
	{}

	std::string GetVertexShader()const { return m_vs; }
	std::string GetFragmentShader()const { return m_fs; }
	Topology GetTopology()const { return m_topology; }
	const std::vector<float> &GetVertices()const { return m_vertices; }
	const std::vector<float> &GetColors()const { return m_colors; }
};
