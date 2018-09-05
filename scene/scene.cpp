#include "scene.h"
#include <Windows.h>
#include <plog/Log.h>
#include <imgui.h>
#include <fstream>
#include <nlohmann/json.hpp>


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

static std::vector<uint8_t> ReadAllBytes(const std::wstring &path)
{
	std::vector<uint8_t> bytes;

	std::ifstream ifs(path, std::ios::binary | std::ios::ate);
	if (ifs) {
		auto pos = ifs.tellg();
		bytes.resize(static_cast<size_t>(pos));

		ifs.seekg(0, std::ios::beg);
		ifs.read((char*)bytes.data(), bytes.size());

		ifs.close();
	}

	return bytes;
}

static bool HasExt(const std::wstring &path, const std::wstring &ext)
{
	if (path.size() < ext.size()) {
		return false;
	}

	auto lhs = path.begin() + path.size() - ext.size();
	auto rhs = ext.begin();
	for (; lhs != path.end(); ++lhs, ++rhs)
	{
		if (tolower(*lhs) != tolower(*rhs)) {
			return false;
		}
	}

	return true;
}


Scene::Scene(const std::string &vs, const std::string &fs)
	: m_vs(vs), m_fs(fs)
{
	m_camera = std::make_shared<PersepectiveCamera>();
	m_cameraNode = Node::Create();
	m_mouseObserver = std::make_shared<OrbitMover>(m_cameraNode);

	Setup();
}

void Scene::Setup()
{
	const auto grid_size = 1.0f;
	const auto grid_count = 5;
	const auto grid_edge = grid_size * grid_count;

	// Axis
	{
		std::vector<float> vertices = {
			// x
			-grid_edge, 0, 0,
			0, 0, 0,

			0, 0, 0,
			grid_edge, 0, 0,
			// y
			0, -grid_edge, 0,
			0, 0, 0,

			0, 0, 0,
			0, grid_edge, 0,
			// z
			0, 0, -grid_edge,
			0, 0, 0,

			0, 0, 0,
			0, 0, grid_edge,
		};

		std::vector<float> colors = {
			// red
			0.5f, 0, 0,
			0.5f, 0, 0,
			1.0f, 0, 0,
			1.0f, 0, 0,
			// green
			0, 0.5f, 0,
			0, 0.5f, 0,
			0, 1.0f, 0,
			0, 1.0f, 0,
			// blue
			0, 0, 0.5f,
			0, 0, 0.5f,
			0, 0, 1.0f,
			0, 0, 1.0f,
		};

		auto node = Node::Create();
		node->SetMesh(std::shared_ptr<Mesh>(new Mesh(m_vs, m_fs, Mesh::Lines, vertices, colors)));
		m_gizmos.push_back(node);
	}

	// Grid
	{
		std::vector<float> vertices;
		std::vector<float> colors;
		for (int i = -grid_count; i <= grid_count; ++i)
		{
			if (i == 0)continue;

			vertices.push_back(-grid_edge);
			vertices.push_back(0);
			vertices.push_back(grid_size*i);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
			colors.push_back(0.5f);

			vertices.push_back(grid_edge);
			vertices.push_back(0);
			vertices.push_back(grid_size*i);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
		}
		for (int i = -grid_count; i <= grid_count; ++i)
		{
			if (i == 0)continue;

			vertices.push_back(grid_size*i);
			vertices.push_back(0);
			vertices.push_back(-grid_edge);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
			colors.push_back(0.5f);

			vertices.push_back(grid_size*i);
			vertices.push_back(0);
			vertices.push_back(grid_edge);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
			colors.push_back(0.5f);
		}

		auto node = Node::Create();
		node->SetMesh(std::shared_ptr<Mesh>(new Mesh(m_vs, m_fs, Mesh::Lines, vertices, colors)));
		m_gizmos.push_back(node);
	}
}

void Scene::CreateDefaultScene()
{
	// triangle
	{
		std::vector<float> vertices = {
			0.0f,  0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f,  0.0f };

		std::vector<float> colors = {
			1.0f, 0, 0,
			0, 1.0f, 0,
			0, 0, 1.0f,
		};

		auto node = Node::Create();
		node->SetMesh(std::shared_ptr<Mesh>(new Mesh(m_vs, m_fs, Mesh::Triangles, vertices, colors)));
		node->SetAnimation(std::make_shared<NodeRotation>(50.0f));
		m_nodes.push_back(node);
	}
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
	ImGui::End();
}

class FileSystem
{
public:
};

void Scene::Load(const std::wstring &path)
{
	if (HasExt(path, L".gltf")) {

		auto json = nlohmann::json::parse(ReadAllBytes(path));
		LOGI << "gltf: " << json;

	}
	else if (HasExt(path, L".glb")
		|| HasExt(path, L".vrm")) {

		auto bytes = ReadAllBytes(path);
		LOGI << bytes.size() << " bytes";

	}
	else {
		LOGW << "unknown file type: " << path;
	}
}
