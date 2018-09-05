#include "gles3renderer.h"
#include "shader.h"
#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <vector>
#include <plog/Log.h>


GLES3Renderer::GLES3Renderer()
{
}

void GLES3Renderer::Resize(int w, int h)
{
	LOGD << "resize: " << w << ", " << h;
	m_width = w;
	m_height = h;
}

void GLES3Renderer::DrawNode(const ICamera *camera, const Node *cameraNode, const Node *node)
{
	auto shader = GetOrCreateShader(node);
	if (!shader) {
		return;
	}

	shader->Use();

	auto projection = camera->GetMatrix();
	auto view = cameraNode->transform;
	auto model = node->transform;

	shader->SetUniformValue("ProjectionMatrix", projection);
	shader->SetUniformValue("ViewMatrix", view);
	shader->SetUniformValue("ModelMatrix", model);

	glm::mat4 mvp = projection * view * model;
	shader->SetUniformValue("MVPMatrix", mvp);

	auto vbo = GetOrCreateVertexArray(node);
	if (vbo) {
		vbo->Draw();
	}
}

void GLES3Renderer::Draw(Scene *pScene)
{
	//
	// rendertarget
	//
	glViewport(0, 0, m_width, m_height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//
	// setup camera
	//
	auto camera = pScene->GetCamera();
	auto cameraNode = pScene->GetCameraNode();

	//
	// Draw
	//
	if (pScene) {
		{
			auto count = pScene->GetGizmosCount();
			for (int i = 0; i < count; ++i) {
				auto node = pScene->GetGizmos(i);
				DrawNode(camera, cameraNode, node);
			}
		}

		{
			auto count = pScene->GetNodeCount();
			for (int i = 0; i < count; ++i) {
				auto node = pScene->GetNode(i);
				DrawNode(camera, cameraNode, node);
			}
		}
	}
}

std::shared_ptr<Shader> GLES3Renderer::GetOrCreateShader(const Node *pNode)
{
	auto found = m_shader_map.find(pNode->GetID());
	if (found != m_shader_map.end()) {
		return found->second;
	}

	auto mesh = pNode->GetMesh();
	if (!mesh)return nullptr;

	auto shader = Shader::Create(mesh->GetVertexShader(), mesh->GetFragmentShader());
	if (shader) {
		m_shader_map.insert(std::make_pair(pNode->GetID(), shader));
	}
	return shader;
}

std::shared_ptr<VertexArray> GLES3Renderer::GetOrCreateVertexArray(const Node *pNode)
{
	auto found = m_vertexbuffer_map.find(pNode->GetID());
	if (found != m_vertexbuffer_map.end()) {
		return found->second;
	}

	auto mesh = pNode->GetMesh();
	if (!mesh)return nullptr;


	std::shared_ptr<VertexArray> vbo;

	switch (mesh->GetTopology()) 
	{
	case Mesh::Triangles:
	{
		vbo = VertexArray::CreateTriangles(mesh->GetVertices(), mesh->GetColors());
		break;
	}

	case Mesh::Lines:
	{
		vbo = VertexArray::CreateLines(mesh->GetVertices(), mesh->GetColors());
		break;
	}
	}
	
	if (vbo) {
		m_vertexbuffer_map.insert(std::make_pair(pNode->GetID(), vbo));
	}
	else {
		LOGE << "fal to create triangles";
	}
	return vbo;
}
