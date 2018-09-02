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
	auto camera = pScene->GetCameraNode();

	//
	// Draw
	//
	if (pScene) {
		auto count = pScene->GetNodeCount();
		for (int i = 0; i < count; ++i) {
			auto node = pScene->GetNode(i);
			
			auto shader = GetOrCreateShader(node);
			if (shader) {
				shader->Use();

				auto projection = camera->GetCamera()->GetMatrix();
				auto view = camera->GetInverse();
				auto model = node->GetTransform();
			
				shader->SetUniformValue("ProjectionMatrix", projection);
				shader->SetUniformValue("ViewMatrix", view);
				shader->SetUniformValue("ModelMatrix", model);

				glm::mat4 mvp = projection * view * model;
				shader->SetUniformValue("MVPMatrix", mvp);

				auto vbo = GetOrCreateVertexArray(node);

				vbo->Draw();
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
	
	auto vbo = VertexArray::Create(mesh->GetVertices());
	m_vertexbuffer_map.insert(std::make_pair(pNode->GetID(), vbo));
	return vbo;
}
