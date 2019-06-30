#include "gles3renderer.h"
#include "shader.h"
#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#include <vector>
#include <plog/Log.h>

namespace agv
{
namespace renderer
{

struct GLES3RendererImpl
{
    std::unordered_map<uint32_t, std::shared_ptr<VertexBufferGroup>> m_vertexBuffer_map;
    std::shared_ptr<VertexBufferGroup> GetOrCreateVertexBuffer(const agv::scene::Mesh *pMesh)
    {
        {
            auto found = m_vertexBuffer_map.find(pMesh->GetID());
            if (found != m_vertexBuffer_map.end())
            {
                return found->second;
            }
        }

        auto vbo = std::make_shared<VertexBufferGroup>(pMesh->GetVertexCount(), pMesh->Topology);
        m_vertexBuffer_map.insert(std::make_pair(pMesh->GetID(), vbo));

        for (auto pair : pMesh->VertexAttributes)
        {
            vbo->AddAttribute(pair.first, pair.second);
        }

        auto &indices = pMesh->Indices;
        if (indices.data)
        {
            vbo->SetIndex(indices);
        }

        return vbo;
    }

    std::unordered_map<uint32_t, std::shared_ptr<Shader>> m_shader_map;
    std::shared_ptr<Shader> GetOrCreateShader(const agv::scene::Material *pMaterial)
    {
        auto found = m_shader_map.find(pMaterial->GetID());
        if (found != m_shader_map.end())
        {
            return found->second;
        }

        auto shader = Shader::Create(*pMaterial);
        if (shader)
        {
            m_shader_map.insert(std::make_pair(pMaterial->GetID(), shader));
        }
        else
        {
            LOGE << "fail to Shader::Create";
        }
        return shader;
    }

    std::unordered_map<uint32_t, std::shared_ptr<VertexArray>> m_vertexArray_map;
    std::shared_ptr<VertexArray> GetOrCreateVertexArray(
        const agv::scene::Submesh *pSubmesh,
        const std::shared_ptr<VertexBufferGroup> &vbo,
        const std::shared_ptr<Shader> &shader)
    {
        {
            auto found = m_vertexArray_map.find(pSubmesh->GetID());
            if (found != m_vertexArray_map.end())
            {
                return found->second;
            }
        }

        auto vao = std::make_shared<VertexArray>();
        m_vertexArray_map.insert(std::make_pair(pSubmesh->GetID(), vao));

        vao->Bind();

        // 使うやつだけVAOに登録する
        auto position = vbo->m_attributes["POSITION"];
        position->Bind();
        vao->BindSlot(0, position);
        if (vbo->m_indices)
        {
            vbo->m_indices->Bind();
        }

        vao->Unbind();

        {
            // cleanup
            if (vbo->m_indices)
            {
                vbo->m_indices->Unbind();
            }
            position->Unbind();
        }

        return vao;
    }
};

GLES3Renderer::GLES3Renderer()
    : m_impl(new GLES3RendererImpl)
{
}

GLES3Renderer::~GLES3Renderer()
{
    delete m_impl;
}

void GLES3Renderer::Resize(int w, int h)
{
    LOGD << "resize: " << w << ", " << h;
    m_width = w;
    m_height = h;
}

void GLES3Renderer::DrawNode(const agv::scene::ICamera *camera, const agv::scene::Node *cameraNode, const agv::scene::Node *node)
{
    auto &mesh = node->Mesh;
    if (mesh)
    {
        auto vbo = m_impl->GetOrCreateVertexBuffer(&*mesh);

        int offset = 0;
        for (auto &submesh : mesh->Submeshes)
        {
            auto shader = m_impl->GetOrCreateShader(&*submesh->GetMaterial());
            if (shader)
            {
                shader->Use();

                auto projection = camera->GetMatrix();
                shader->SetUniformValue("ProjectionMatrix", projection);

                auto view = cameraNode->transform;
                shader->SetUniformValue("ViewMatrix", view);

                auto model = node->transform;
                shader->SetUniformValue("ModelMatrix", model);

                glm::mat4 mvp = projection * view * model;
                shader->SetUniformValue("MVPMatrix", mvp);

                auto vao = m_impl->GetOrCreateVertexArray(&*submesh, vbo, shader);
                vao->Bind();
                vbo->Draw(offset, submesh->GetDrawCount());
            }
            offset += submesh->GetDrawCount();
        }
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
    if (pScene)
    {
        {
            auto count = pScene->GetGizmosCount();
            for (int i = 0; i < count; ++i)
            {
                auto node = pScene->GetGizmos(i);
                DrawNode(camera, cameraNode, node);
            }
        }

        {
            auto count = pScene->GetNodeCount();
            for (int i = 0; i < count; ++i)
            {
                auto node = pScene->GetNode(i);
                DrawNode(camera, cameraNode, node);
            }
        }
    }
}

} // namespace renderer
} // namespace agv