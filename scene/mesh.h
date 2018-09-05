#pragma once
#include <string>
#include <vector>
#include "material.h"


class Mesh
{
public:
	enum Topology
	{
		Triangles,
		Lines,
	};

private:

	Material m_material;
	Topology m_topology;
	std::vector<float> m_vertices;
	std::vector<float> m_colors;

public:
	Mesh(const Material &material,
		Topology topology,
		const std::vector<float> &vertices,
		const std::vector<float> &colors
		)
		: m_material(material), m_topology(topology),
		m_vertices(vertices), m_colors(colors)
	{}

	const Material& GetMaterial()const { return m_material; }
	Topology GetTopology()const { return m_topology; }
	const std::vector<float> &GetVertices()const { return m_vertices; }
	const std::vector<float> &GetColors()const { return m_colors; }
};
