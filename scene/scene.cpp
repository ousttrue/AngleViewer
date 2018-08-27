#include "scene.h"


void Scene::AddTriangle(const std::string &vs, const std::string &fs)
{
	std::vector<float> vertices = { 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f,  0.0f };
	m_nodes.push_back(std::make_shared<Node>(m_next_unique_id++,
		vs, fs, vertices
		));
}

void Scene::Update()
{
	for (auto node : m_nodes) {
		node->Update();
	}
}
