#include "scene.h"
#include "camera.h"
#include <Windows.h>
#include <plog/Log.h>


Scene::Scene()
{
	auto node = Node::Create();
	auto camera = std::make_shared<PersepectiveCamera>();
	node->SetCamera(camera);
	node->position = glm::vec3(0, 0, 2.0f);
	m_cameraNode = node;
	//m_nodes.push_back(node);
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
		node->SetAnimation(std::make_shared<NodeRotation>(50.0f));
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

const float RORATION_SPEED = 0.1f;

void Scene::MouseMove(int x, int y)
{
	auto dx = x - m_mouseLastX;
	auto dy = y - m_mouseLastY;
	m_mouseLastX = x;
	m_mouseLastY = y;

	if (m_mouseLeftIsDown) {

	}
	if (m_mouseMiddleIsDown) {

	}
	if (m_mouseRightIsDown) {
		m_cameraNode->eulerRadians.y += glm::radians((float)dx * RORATION_SPEED);
		m_cameraNode->eulerRadians.x += glm::radians((float)dy * RORATION_SPEED);
	}
}

void Scene::MouseLeftDown()
{
	m_mouseLeftIsDown = true;
}

void Scene::MouseLeftUp()
{
	m_mouseLeftIsDown = false;
}

void Scene::MouseMiddleDown()
{
	m_mouseMiddleIsDown = true;
}

void Scene::MouseMiddleUp()
{
	m_mouseMiddleIsDown = false;
}

void Scene::MouseRightDown()
{
	m_mouseRightIsDown = true;
}

void Scene::MouseRightUp()
{
	m_mouseRightIsDown = false;
}

void Scene::MouseWheel(int d)
{
	m_cameraNode->ForwardWheel(d);
}
