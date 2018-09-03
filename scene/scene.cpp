#include "scene.h"
#include <Windows.h>
#include <plog/Log.h>


Scene::Scene()
{
	m_camera = std::make_shared<PersepectiveCamera>();
	m_cameraNode = Node::Create();
	m_mouseObserver = std::make_shared<OrbitMover>(m_cameraNode);
}

void Scene::Setup(const std::string &vs, const std::string &fs)
{
	// Axis
	{
		std::vector<float> vertices = {
			// x
			-1.0f, 0, 0,
			0, 0, 0,

			0, 0, 0,
			1.0f, 0, 0,
			// y
			0, -1.0f, 0,
			0, 0, 0,

			0, 0, 0,
			0, 1.0f, 0,
			// z
			0, 0, -1.0f,
			0, 0, 0,

			0, 0, 0,
			0, 0, 1.0f,
		};

		std::vector<float> colors = {
			// red
			0.5f, 0, 0,
			0.5f, 0, 0,
			1.0f, 0, 0,
			1.0f, 0, 0,
			// green
			0, 0.5f, 0,
			0, 0.5f, 0,
			0, 1.0f, 0,
			0, 1.0f, 0,
			// blue
			0, 0, 0.5f,
			0, 0, 0.5f,
			0, 0, 1.0f,
			0, 0, 1.0f,
		};

		auto node = Node::Create();
		node->SetMesh(std::shared_ptr<Mesh>(new Mesh(vs, fs, Mesh::Lines, vertices, colors)));
		m_nodes.push_back(node);
	}

	// triangle
	{
		std::vector<float> vertices = { 
			0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f,  0.0f };

		std::vector<float> colors = {
			1.0f, 0, 0,
			0, 1.0f, 0,
			0, 0, 1.0f,
		};

		auto node = Node::Create();
		node->SetMesh(std::shared_ptr<Mesh>(new Mesh(vs, fs, Mesh::Triangles, vertices, colors)));
		node->SetAnimation(std::make_shared<NodeRotation>(50.0f));
		m_nodes.push_back(node);
	}
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

void Scene::SetScreenSize(int w, int h)
{
	m_camera->SetScreenSize(w, h);
}
