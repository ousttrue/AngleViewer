#include "scene.h"
#include "node.h"
#include <Windows.h>
#include <plog/Log.h>
#include <imgui.h>
#include <fstream>

static std::wstring OpenDialog()
{
    OPENFILENAME ofn;        // common dialog box structure
    TCHAR szFile[260] = {0}; // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileName(&ofn) == TRUE)
    {
        // use ofn.lpstrFile
        return L"";
    }

    return szFile;
}

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

static void DrawNodeRecursive(const std::shared_ptr<Node> &node)
{
    ImGui::PushID(&node);
    bool isOpen = ImGui::TreeNode("%s", node->GetName().c_str());
    if (isOpen)
    {

        for (auto &child : node->GetChildren())
        {
            DrawNodeRecursive(child);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
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

    ImGui::Begin("scene", nullptr, ImGuiWindowFlags_MenuBar);
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    auto path = OpenDialog();
                    if (!path.empty())
                    {
                        Load(path);
                    }
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("time: %d", now);
        ImGui::Text("fps: %d", m_fps);

        if (m_model)
        {
            for (auto &node : m_model->Root->GetChildren())
            {
                DrawNodeRecursive(node);
            }
        }

        ImGui::End();
    }

    ImGui::Begin("gltf");
    {
        if (m_model)
        {
            //ImGui::Text("generator: %s", m_gltf->asset.generator.c_str());

            //ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
            if (ImGui::TreeNode("nodes"))
            {

                ImGui::Columns(2);
                for (int i = 0; i < m_model->Nodes.size(); ++i)
                {
                    auto &node = m_model->Nodes[i];
                    ImGui::PushID(node->GetName().c_str());

                    bool isOpen = ImGui::TreeNode("%s", node->GetName().c_str());
                    ImGui::NextColumn();
                    if (isOpen)
                    {
                        ImGui::Text("%03d", i);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();

                    ++i;
                }
                ImGui::Columns(1);

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

    //ImGui::ShowDemoWindow();
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
