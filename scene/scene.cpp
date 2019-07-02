#include "scene.h"
#include "node.h"
#include <plog/Log.h>
#include <fstream>

namespace agv
{
namespace scene
{
Scene::Scene()
{
    m_gizmoMaterial = std::make_shared<Material>("_gizmo");

    m_camera = std::make_shared<PerspectiveCamera>();
    m_cameraNode = std::make_shared<Node>("_camera");
    m_mouseObserver = std::make_shared<OrbitMover>(m_cameraNode);

    Setup();
}

void Scene::Setup()
{
    const auto grid_size = 1.0f;
    const auto grid_count = 5;
    const auto grid_edge = grid_size * grid_count;

    {
        auto axis = std::make_shared<Node>("_axis");
        auto mesh = Mesh::CreateAxis(grid_edge);
        mesh->WholeSubmesh(m_gizmoMaterial);
        axis->MeshGroup = MeshGroup::Create(mesh);
        m_gizmos.push_back(axis);
    }
    {
        auto grid = std::make_shared<Node>("_grid");
        auto mesh = Mesh::CreateGrid(grid_size, grid_count);
        mesh->WholeSubmesh(m_gizmoMaterial);
        grid->MeshGroup = MeshGroup::Create(mesh);
        m_gizmos.push_back(grid);
    }
}

void Scene::CreateDefaultScene()
{
    auto node = std::make_shared<Node>("_triangle");
    auto mesh = Mesh::CreateSampleTriangle(1.0f);
    mesh->WholeSubmesh(m_gizmoMaterial);
    node->MeshGroup = MeshGroup::Create(mesh);
    node->Animation = std::make_shared<NodeRotation>(50.0f);
    m_model = std::make_shared<Model>();
    m_model->Root->AddChild(node);
}

void Scene::Update(uint32_t now)
{
    auto delta = m_time == 0 ? 0 : now - m_time;
    m_time = now;

    m_frameCount++;
    auto seconds = now / 1000;
    if (m_seconds != seconds)
    {
        m_fps = m_frameCount;
        m_frameCount = 0;
        m_seconds = seconds;
    }

    auto time = AnimationTime{
        now * 0.001f,
        delta * 0.001f,
    };

    if (m_model)
    {
        m_model->SetTime(time);
    }
}

void Scene::Load(const std::wstring &path)
{
    simplegltf::Storage storage;
    if (!storage.from_path(path))
    {
        LOGE << "fail to load: " << path;
        return;
    }
    LOGI << "load: " << path;

    m_model = Model::Load(storage);
}
} // namespace scene
} // namespace agv
