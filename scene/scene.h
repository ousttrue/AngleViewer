#pragma once
#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define _USE_MATH_DEFINES
#include <math.h>


const auto TO_RAD = M_PI / 180.0;

class Node
{
	uint32_t m_unique_id=0;
	std::string m_vs;
	std::string m_fs;
	std::vector<float> m_vertices;

	glm::mat4 m_transform = glm::identity<glm::mat4>();

public:
	Node(uint32_t unique_id, 
		const std::string &vs, 
		const std::string &fs,
		const std::vector<float> &vertices
	)
		: m_unique_id(unique_id)
		, m_vs(vs)
		, m_fs(fs)
		, m_vertices(vertices)
	{}

	uint32_t GetID()const { return m_unique_id; }
	std::string GetVertexShader()const { return m_vs; }
	std::string GetFragmentShader()const { return m_fs; }
	std::vector<float> GetVertices()const { return m_vertices; }
	glm::mat4 GetTransform()const { return m_transform; }

	float m_angle = 0;
	void Update()
	{
		m_angle += 1 * TO_RAD;
		m_transform = glm::rotate(glm::mat4(1.0f), m_angle, glm::vec3(0.0f, 0.0f, 1.0f));
	}
};


class Scene
{
	std::vector<std::shared_ptr<Node>> m_nodes;
	uint32_t m_next_unique_id = 1;

public:
	void AddTriangle(const std::string &vs, const std::string &fs);
	void Update();

	int GetNodeCount()const { return static_cast<int>(m_nodes.size()); }
	const Node* GetNode(int index)const
	{
		if (index < 0 || index >= m_nodes.size())return nullptr;
		return &*m_nodes[index];
	}
};
