#include "scene.h"
#include "loader.h"
#include <Windows.h>
#include <plog/Log.h>
#include <imgui.h>
#include <fstream>
#include <GLTFSDK/Document.h>


static std::wstring OpenDialog()
{
	OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros

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


namespace agv {
	namespace scene {
		Scene::Scene(const renderer::Material &material)
			: m_material(material)
		{
			m_camera = std::make_shared<PersepectiveCamera>();
			m_cameraNode = Node::Create("_camera");
			m_mouseObserver = std::make_shared<OrbitMover>(m_cameraNode);

			Setup();
		}

		void Scene::Setup()
		{
			const auto grid_size = 1.0f;
			const auto grid_count = 5;
			const auto grid_edge = grid_size * grid_count;

			m_gizmos.push_back(Node::Create("_axis", Mesh::CreateAxis(m_material, grid_edge)));
			m_gizmos.push_back(Node::Create("_grid", Mesh::CreateGrid(m_material, grid_size, grid_count)));
		}

		void Scene::CreateDefaultScene()
		{
			auto node = Node::Create("_triangle", Mesh::CreateSampleTriangle(m_material, 1.0f));
			node->SetAnimation(std::make_shared<NodeRotation>(50.0f));
			m_nodes.push_back(node);
		}

		static void DrawNodeRecursive(const std::shared_ptr<Node> &node)
		{
			ImGui::PushID(&node);
			bool isOpen = ImGui::TreeNode("%s", node->GetName().c_str());
			if (isOpen) {

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
			if (m_seconds != seconds) {
				m_fps = m_frameCount;
				m_frameCount = 0;
				m_seconds = seconds;
			}

			auto time = AnimationTime{
				now * 0.001f,
				delta * 0.001f,
			};
			for (auto node : m_nodes) {
				node->Update(time);
			}

			ImGui::Begin("scene", nullptr, ImGuiWindowFlags_MenuBar);
			{
				if (ImGui::BeginMenuBar()) {
					if (ImGui::BeginMenu("File")) {
						if (ImGui::MenuItem("Open")) {
							auto path = OpenDialog();
							if (!path.empty()) {

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
				if (m_gltf) {
					ImGui::Text("generator: %s", m_gltf->asset.generator.c_str());

					//ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
					if (ImGui::TreeNode("nodes")) {

						ImGui::Columns(2);
						int i = 0;
						for (auto &node : m_gltf->nodes.Elements())
						{
							ImGui::PushID(&node);

							bool isOpen = ImGui::TreeNode("%s", node.name.c_str());
							ImGui::NextColumn();
							if (isOpen) {
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

			ImGui::ShowDemoWindow();
		}

		void Scene::Load(const std::wstring &path)
		{
			m_gltf = LoadGLTF(path);
			if (!m_gltf)return;

			m_nodes.clear();

			for (auto &gltfNode : m_gltf->nodes.Elements()) {

				//LOGD << node.name;
				auto node = Node::Create(gltfNode.name);

				if (!gltfNode.meshId.empty()) {
					auto meshIndex = atoi(gltfNode.meshId.c_str());
					auto mesh = Mesh::CreateFromGltf(m_material, m_gltf, meshIndex);
					node->SetMesh(mesh);
				}

				m_nodes.push_back(node);

			}

			// build tree
			int i = 0;
			for (auto &gltfNode : m_gltf->nodes.Elements()) {

				auto &n = m_nodes[i++];
				for (auto child : gltfNode.children)
				{
					auto child_index = atoi(child.c_str());
					n->AddChild(m_nodes[child_index]);
				}

			}
		}
	}
}
