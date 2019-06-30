#include "node.h"
#include <plog/Log.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

namespace agv
{
namespace scene
{

std::shared_ptr<Node> Node::Load(const simplegltf::Storage &storage,
                                 const simplegltf::GltfNode &gltfNode)
{
	auto &gltf = storage.gltf;
	auto node = std::make_shared<Node>(gltfNode.name);
	return node;
}

} // namespace scene
} // namespace agv