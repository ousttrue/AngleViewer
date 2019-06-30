#include "gles3renderer.h"
#include "shader.h"
#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <vector>
#include <plog/Log.h>


namespace agv {
	namespace renderer {
		GLES3Renderer::GLES3Renderer()
		{
		}

		void GLES3Renderer::Resize(int w, int h)
		{
			LOGD << "resize: " << w << ", " << h;
			m_width = w;
			m_height = h;
		}

		void GLES3Renderer::DrawNode(const agv::scene::ICamera *camera, const agv::scene::Node *cameraNode, const agv::scene::Node *node)
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

		void GLES3Renderer::Draw(agv::scene::Scene *pScene)
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

		std::shared_ptr<Shader> GLES3Renderer::GetOrCreateShader(const agv::scene::Node *pNode)
		{
			auto found = m_shader_map.find(pNode->GetID());
			if (found != m_shader_map.end()) {
				return found->second;
			}

			auto mesh = pNode->GetMesh();
			if (!mesh)return nullptr;

			auto shader = Shader::Create(mesh->GetMaterial());
			if (shader) {
				m_shader_map.insert(std::make_pair(pNode->GetID(), shader));
			}
			return shader;
		}

		std::shared_ptr<VertexArray> GLES3Renderer::GetOrCreateVertexArray(const agv::scene::Node *pNode)
		{
			auto found = m_vertexbuffer_map.find(pNode->GetID());
			if (found != m_vertexbuffer_map.end()) {
				return found->second;
			}

			auto mesh = pNode->GetMesh();
			if (!mesh)return nullptr;


			std::shared_ptr<VertexArray> vao;

			switch (mesh->GetTopology())
			{
			case scene::Mesh::Triangles:
			{
				vao = VertexArray::CreateTriangles(mesh->GetVertices().get_count(),
					(float*)mesh->GetVertices().data,
					(float*)mesh->GetColors().data);
				break;
			}

			case scene::Mesh::Lines:
			{
				vao = VertexArray::CreateLines(mesh->GetVertices().get_count(),
					(float*)mesh->GetVertices().data,
					(float*)mesh->GetColors().data);
				break;
			}
			}

			if (vao) {
				auto &indices = mesh->GetIndices();
				if (indices.data) {
					auto ibo = std::make_shared<VertexBuffer>();
					ibo->BufferData(true, indices.data, indices.size);
					switch (indices.valuetype)
					{
					case simplegltf::ValueType::UInt16:
						vao->SetIndex(ibo, indices.get_count(), GL_UNSIGNED_SHORT);
						break;

					case simplegltf::ValueType::UInt32:
						vao->SetIndex(ibo, indices.get_count(), GL_UNSIGNED_INT);
						break;
					}
				}

				m_vertexbuffer_map.insert(std::make_pair(pNode->GetID(), vao));
			}
			else {
				LOGE << "fal to create triangles";
			}
			return vao;
		}
	}
}