#include "scene.h"
#include "camera.h"
#include <Windows.h>


Scene::Scene()
{
	auto node = Node::Create();
	auto camera = std::make_shared<PersepectiveCamera>();
	node->SetCamera(camera);
	m_nodes.push_back(node);
}

void Scene::AddTriangle(const std::string &vs, const std::string &fs)
{
	std::vector<float> vertices = { 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f,  0.0f };
	auto node = Node::Create();
	node->SetMesh(std::shared_ptr<Mesh>(new Mesh(vs, fs, vertices)));
}

void Scene::Update(uint32_t now)
{
	auto delta = m_time == 0 ? 0 : now - m_time;
	m_time = now;

	auto time = AnimationTime{
		now * 0.001f,
		delta * 0.001f,
	};
	for (auto node : m_nodes) {
		node->Update(time);
	}
}
