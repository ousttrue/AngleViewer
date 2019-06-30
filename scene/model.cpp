#include "model.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "node.h"

namespace agv
{
namespace scene
{

Model::Model()
    : Root(new Node("__root__"))
{
}

void Model::SetTime(const AnimationTime &time)
{
    for (auto &node : Nodes)
    {
        auto &animation = node->Animation;
        if (animation)
        {
            animation->Update(&*node, time);
        }
    }
}

std::shared_ptr<Model> Model::Load(const simplegltf::Storage &storage)
{
    auto model = std::make_shared<Model>();
    auto &gltf = storage.gltf;

    for (auto &gltfTexture : gltf.textures)
    {
        model->Textures.push_back(Texture::Load(storage, gltfTexture));
    }

    for (auto &gltfMaterial : gltf.materials)
    {
        model->Materials.push_back(Material::Load(storage, gltfMaterial, model->Textures));
    }

    for (auto &mesh : gltf.meshes)
    {
        model->Meshes.push_back(MeshGroup::Load(storage, mesh, model->Materials));
    }

    for (auto &gltfNode : gltf.nodes)
    {
        model->Nodes.push_back(Node::Load(storage, gltfNode));
    }

    // build tree
    for (int i = 0; i < gltf.nodes.size(); ++i)
    {
        auto &gltfNode = gltf.nodes[i];
        auto &node = model->Nodes[i];

        if (gltfNode.mesh >= 0)
        {
            node->MeshGroup = model->Meshes[gltfNode.mesh];
        }

        for (auto childIndex : gltf.nodes[i].children)
        {
            node->AddChild(model->Nodes[childIndex]);
        }
    }

    for (auto &node : model->Nodes)
    {
        if (!node->GetParent())
        {
            model->Root->AddChild(node);
        }
    }

    return model;
}

} // namespace scene
} // namespace agv