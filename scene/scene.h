#pragma once
#include "node.h"


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
