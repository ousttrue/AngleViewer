#pragma once
#include "objectbase.h"
#include "animation_time.h"
#include "camera.h"
#include "node_animation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace agv
{
namespace scene
{

class MeshGroup;
class Node : public ObjectBase
{
public:
    Node(const std::string name) : ObjectBase(name)
    {
    }

    glm::mat4 transform = glm::identity<glm::mat4>();
    std::shared_ptr<MeshGroup> MeshGroup;
    std::shared_ptr<INodeAnimation> Animation;

private:
    Node *m_parent = nullptr;
    std::vector<std::shared_ptr<Node>> m_children;

public:
    Node *GetParent()
    {
        return m_parent;
    }

    std::vector<std::shared_ptr<Node>> &GetChildren()
    {
        return m_children;
    }

    void AddChild(const std::shared_ptr<Node> &child)
    {
        assert(!child->m_parent);
        child->m_parent = this;
        m_children.push_back(child);
    }

    static std::shared_ptr<Node> Load(const simplegltf::Storage &storage,
                                      const simplegltf::GltfNode &gltfNode);
};
} // namespace scene
} // namespace agv
