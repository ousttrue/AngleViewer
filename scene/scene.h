#pragma once
#include "camera.h"
#include "mouse.h"
#include "mesh.h"
#include "node_animation.h"
#include "node.h"


class Scene
{
	std::shared_ptr<ICamera> m_camera;
	std::shared_ptr<Node> m_cameraNode;

	std::vector<std::shared_ptr<Node>> m_nodes;

	std::shared_ptr<IMouseObserver> m_mouseObserver;

	uint32_t m_time = 0;

public:
	Scene();

	void Setup(const std::string &vs, const std::string &fs);
	void Update(uint32_t now);

	int GetNodeCount()const { return static_cast<int>(m_nodes.size()); }

	const ICamera* GetCamera()const
	{
		if (!m_camera)return nullptr;
		return &*m_camera;
	}

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

	IMouseObserver* GetMouseObserver()
	{
		if (!m_mouseObserver)return nullptr;
		return &*m_mouseObserver;
	}

	void SetScreenSize(int w, int h);
};
