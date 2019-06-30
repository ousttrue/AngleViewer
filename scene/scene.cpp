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
Scene::Scene(const std::shared_ptr<scene::Material> &material)
    : m_material(material)
{
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
        axis->Mesh = Mesh::CreateAxis(m_material, grid_edge);
        m_gizmos.push_back(axis);
    }
    {
        auto grid = std::make_shared<Node>("_grid");
        grid->Mesh = Mesh::CreateGrid(m_material, grid_size, grid_count);
        m_gizmos.push_back(grid);
    }
}

void Scene::CreateDefaultScene()
{
    auto node = std::make_shared<Node>("_triangle");
    node->Mesh = Mesh::CreateSampleTriangle(m_material, 1.0f);
    node->Animation = std::make_shared<NodeRotation>(50.0f);
    m_nodes.push_back(node);
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
    for (auto node : m_nodes)
    {
        auto &animation = node->Animation;
        if(animation){
            animation->Update(&*node, time);
        }
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

        for (auto &node : m_nodes)
        {
            if (!node->GetParent())
            {
                DrawNodeRecursive(node);
            }
        }

        ImGui::End();
    }

    ImGui::Begin("gltf");
    {
        if (!m_storage.buffers.empty())
        {
            //ImGui::Text("generator: %s", m_gltf->asset.generator.c_str());
            auto &gltf = m_storage.gltf;

            //ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
            if (ImGui::TreeNode("nodes"))
            {

                ImGui::Columns(2);
                auto nodeCount = gltf.nodes.size();
                for (int i = 0; i < nodeCount; ++i)
                {
                    auto nodeName = gltf.nodes[i].name;
                    ImGui::PushID(nodeName.c_str());

                    bool isOpen = ImGui::TreeNode("%s", nodeName.c_str());
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
    m_nodes.clear();

    if (!m_storage.from_path(path))
    {
        return;
    }
    LOGI << "load: " << path;
    auto &gltf = m_storage.gltf;

    for (auto &gltfMaterial : gltf.materials)
    {
    }

    for (auto &gltfNode : gltf.nodes)
    {
        LOGD << gltfNode.name;
        auto node = std::make_shared<Node>(gltfNode.name);

        if (gltfNode.mesh >= 0)
        {
            auto &gltfMesh = m_storage.gltf.meshes[gltfNode.mesh];
            for (int i = 0; i < gltfMesh.primitives.size(); ++i)
            {
                auto mesh = std::make_shared<Mesh>(gltfMesh.name);
                mesh->Material = m_material;

                auto &primitive = gltfMesh.primitives[i];
                for (auto pair : primitive.attributes)
                {
                    mesh->AddVertexAttribute(pair.first, m_storage.get_from_accessor(pair.second));
                }
                mesh->Indices = m_storage.get_from_accessor(primitive.indices);

                node->Mesh = mesh;
            }
        }

        m_nodes.push_back(node);
    }

    // build tree
    for (int i = 0; i < gltf.nodes.size(); ++i)
    {
        auto &n = m_nodes[i];
        for (auto childIndex : gltf.nodes[i].children)
        {
            n->AddChild(m_nodes[childIndex]);
        }
    }
}
} // namespace scene
} // namespace agv
