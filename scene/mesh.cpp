#include "mesh.h"
#include <plog/Log.h>
#include <glm/glm.hpp>

namespace agv
{
namespace scene
{
void Mesh::WholeSubmesh(const std::shared_ptr<Material> &material)
{
    int drawCount = Indices.data ? Indices.get_count() : VertexAttributes["POSITION"].get_count();
    auto submesh = std::make_shared<Submesh>(GetName() + "#whole", drawCount, material);
    Submeshes.push_back(submesh);
}

std::shared_ptr<Mesh> Mesh::CreateGrid(float grid_size, int grid_count)
{
    auto grid_edge = grid_size * grid_count;
    auto vertices = std::make_shared<simplegltf::Buffer>();
    auto colors = std::make_shared<simplegltf::Buffer>();
    for (int i = -grid_count; i <= grid_count; ++i)
    {
        if (i == 0)
            continue;

        vertices->push_back(glm::vec3(-grid_edge, 0, grid_size * i));
        colors->push_back(glm::vec3(0.5f, 0.5, 0.5f));

        vertices->push_back(glm::vec3(grid_edge, 0, grid_size * i));
        colors->push_back(glm::vec3(0.5f, 0.5, 0.5f));
    }
    for (int i = -grid_count; i <= grid_count; ++i)
    {
        if (i == 0)
            continue;

        vertices->push_back(glm::vec3(grid_size * i, 0, -grid_edge));
        colors->push_back(glm::vec3(0.5f, 0.5f, 0.5f));

        vertices->push_back(glm::vec3(grid_size * i, 0, grid_edge));
        colors->push_back(glm::vec3(0.5f, 0.5f, 0.5f));
    }

    auto mesh = std::make_shared<Mesh>("grid");
    mesh->Topology = simplegltf::GltfTopologyType::LINES;
    mesh->AddVertexAttribute("POSITION", simplegltf::View(vertices, simplegltf::ValueType::FloatVec3));
    mesh->AddVertexAttribute("COLOR_0", simplegltf::View(colors, simplegltf::ValueType::FloatVec3));
    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateAxis(float grid_edge)
{
    glm::vec3 vertices[] = {
        // x
        {-grid_edge, 0, 0},
        {0, 0, 0},

        {0, 0, 0},
        {grid_edge, 0, 0},
        // y
        {0, -grid_edge, 0},
        {0, 0, 0},

        {0, 0, 0},
        {0, grid_edge, 0},
        // z
        {0, 0, -grid_edge},
        {0, 0, 0},

        {0, 0, 0},
        {0, 0, grid_edge},
    };

    glm::vec3 colors[] = {
        // red
        {0.5f, 0, 0},
        {0.5f, 0, 0},
        {1.0f, 0, 0},
        {1.0f, 0, 0},
        // green
        {0, 0.5f, 0},
        {0, 0.5f, 0},
        {0, 1.0f, 0},
        {0, 1.0f, 0},
        // blue
        {0, 0, 0.5f},
        {0, 0, 0.5f},
        {0, 0, 1.0f},
        {0, 0, 1.0f},
    };

    auto mesh = std::make_shared<Mesh>("axis");
    mesh->Topology = simplegltf::GltfTopologyType::LINES;

    mesh->AddVertexAttribute("POSITION", simplegltf::View::copy(
                                             vertices, simplegltf::ValueType::FloatVec3));
    mesh->AddVertexAttribute("COLOR_0", simplegltf::View::copy(
                                            colors, simplegltf::ValueType::FloatVec3));
    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateSampleTriangle(float size)
{
    glm::vec3 vertices[] = {
        {0.0f, size, 0.0f},
        {-size, -size, 0.0f},
        {size, -size, 0.0f}};

    glm::vec3 colors[] = {
        {1.0f, 0, 0},
        {0, 1.0f, 0},
        {0, 0, 1.0f},
    };

    auto mesh = std::make_shared<Mesh>("triangle");
    mesh->AddVertexAttribute("POSITION", simplegltf::View::copy(vertices, simplegltf::ValueType::FloatVec3));
    mesh->AddVertexAttribute("COLOR_0", simplegltf::View::copy(colors, simplegltf::ValueType::FloatVec3));
    return mesh;
}

} // namespace scene
} // namespace agv
