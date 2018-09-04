#include "gui.h"
#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
const char* glsl_version = "#version 300 es";


GUI::GUI()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
	io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)
}

GUI::~GUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui::DestroyContext();
}

void GUI::MouseMove(int x, int y)
{
	ImGuiIO& io = ImGui::GetIO();
	// Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
	io.MousePos = ImVec2(x, y);
}

void GUI::MouseLeftDown()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = true;
}

void GUI::MouseLeftUp()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[0] = false;
}

void GUI::MouseMiddleDown()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[2] = true;
}

void GUI::MouseMiddleUp()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[2] = false;
}

void GUI::MouseRightDown()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[1] = true;
}

void GUI::MouseRightUp()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDown[1] = false;
}

void GUI::MouseWheel(int d)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += d;
}

void GUI::SetScreenSize(int w, int h)
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)w, (float)h);
}

void GUI::Render(Scene *pScene, float deltaSeconds)
{
	if (!m_initialized) {
		//ImGui_ImplOpenGL3_CreateFontsTexture();

		ImGui_ImplOpenGL3_Init(glsl_version);
		m_initialized = true;
	}

	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();

	ImGuiIO& io = ImGui::GetIO();
	IM_ASSERT(io.Fonts->IsBuilt());
	io.DeltaTime = deltaSeconds;

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
