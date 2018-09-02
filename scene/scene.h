#pragma once
#include "camera.h"
#include "mesh.h"
#include "node_animation.h"
#include "node.h"


class Scene
{
	std::shared_ptr<Node> m_cameraNode;
	std::vector<std::shared_ptr<Node>> m_nodes;
	uint32_t m_time = 0;

public:
	Scene();

	void AddTriangle(const std::string &vs, const std::string &fs);
	void Update(uint32_t now);

	int GetNodeCount()const { return static_cast<int>(m_nodes.size()); }

	const Node* GetCameraNode()const
	{
		if (!m_cameraNode)return nullptr;
		return &*m_cameraNode;
	}

	const Node* GetNode(int index)const
	{
		if (index < 0 || index >= m_nodes.size())return nullptr;
		return &*m_nodes[index];
	}

	void MouseMove(int x, int y);
	void MouseLeftDown();
	void MouseLeftUp();
	void MouseMiddleDown();
	void MouseMiddleUp();
	void MouseRightDown();
	void MouseRightUp();
	void MouseWheel(int d);
};
