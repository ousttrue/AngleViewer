#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>


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
	void Update();
};

