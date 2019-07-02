#include "guistate.h"
#include <scene.h>
#include <imgui.h>
#include <memory>
#include <Windows.h>


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


static void DrawNodeRecursive(const std::shared_ptr<agv::scene::Node> &node)
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


void GuiState::Update(agv::scene::Scene *scene)
{
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
                        scene->Load(path);
                    }
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("time: %d", scene->GetTime());
        ImGui::Text("fps: %d", scene->GetFps());

        auto model = scene->GetModel();
        if (model)
        {
            for (auto &node : model->Root->GetChildren())
            {
                DrawNodeRecursive(node);
            }
        }

        ImGui::End();
    }

    ImGui::Begin("assets");
    {
        auto model = scene->GetModel();
        if (model)
        {
            //ImGui::Text("generator: %s", m_gltf->asset.generator.c_str());

            //ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
            if (ImGui::TreeNode("textures"))
            {
                auto &textures = model->Textures;
                for (int i = 0; i < textures.size(); ++i)
                {
                    auto &texture = textures[i];
                    ImGui::PushID(texture->GetID());
                    bool isOpen = ImGui::TreeNode("%s", texture->GetName().c_str());
                    if(isOpen)
                    {
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("materials"))
            {
                auto &materials = model->Materials;
                for (int i = 0; i < materials.size(); ++i)
                {
                    auto &material = materials[i];
                    ImGui::PushID(material->GetID());
                    bool isOpen = ImGui::TreeNode("%s", material->GetName().c_str());
                    if(isOpen)
                    {
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("meshes"))
            {
                auto &meshes = model->Meshes;
                for (int i = 0; i < meshes.size(); ++i)
                {
                    auto &mesh = meshes[i];
                    ImGui::PushID(mesh->GetID());
                    bool isOpen = ImGui::TreeNode("%s", mesh->GetName().c_str());
                    if(isOpen)
                    {
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("nodes"))
            {

                ImGui::Columns(2);
                for (int i = 0; i < model->Nodes.size(); ++i)
                {
                    auto &node = model->Nodes[i];
                    ImGui::PushID(node->GetID());

                    bool isOpen = ImGui::TreeNode("%s", node->GetName().c_str());
                    ImGui::NextColumn();
                    if (isOpen)
                    {
                        ImGui::Text("%03d", i);
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);

                ImGui::TreePop();
            }
        }
        ImGui::End();
    }

    ImGui::ShowDemoWindow();

    ImGui::Begin("logger", &loggerOpen);
    {
        ImGui::BeginChild("scrolling");
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
        //if (copy) ImGui::LogToClipboard();

        /*
				if (Filter.IsActive())
				{
					const char* buf_begin = Buf.begin();
					const char* line = buf_begin;
					for (int line_no = 0; line != NULL; line_no++)
					{
						const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
						if (Filter.PassFilter(line, line_end))
							ImGui::TextUnformatted(line, line_end);
						line = line_end && line_end[1] ? line_end + 1 : NULL;
					}
				}
				else
				*/
        {
            ImGui::TextUnformatted(logger.c_str());
        }

        ImGui::SetScrollHere(1.0f);

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::End();
    }
}
