#include "gui.h"
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
const char* glsl_version = "#version 300 es";


GUI::GUI()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

	//ImGui_ImplOpenGL3_CreateFontsTexture();

	ImGui_ImplOpenGL3_Init(glsl_version);
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void GUI::Render(Scene *pScene, int w, int h)
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt());
	int display_w = w;
	int display_h = h;
	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	ImGui::NewFrame();

	// GUI
	ImGui::Text("Hello, world %d", 123);
	if (ImGui::Button("Save"))
	{
		// do stuff
	}
	/*
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	*/

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
