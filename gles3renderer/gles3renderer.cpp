#include "gles3renderer.h"
#include "gles3vertexbuffer.h"
#include "gles3material.h"
#include "gles3shader.h"
#include "gles3texture.h"
#include "scene.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h>
#include <vector>
#include <exception>
#include <plog/Log.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct StbImage
{
    unsigned char *data = nullptr;
    int width = 0;
    int height = 0;
    int channels = 0;

    int size() const
    {
        return width * height * channels;
    }

    bool Load(const std::byte *src, size_t size)
    {
        data = stbi_load_from_memory(reinterpret_cast<const stbi_uc *>(src), static_cast<int>(size),
                                     &width, &height, &channels, 4);
        return data != 0;
    }

    ~StbImage()
    {
        if (data)
        {
            stbi_image_free(data);
            data = 0;
        }
    }
};

namespace agv
{
namespace renderer
{

///
/// resource manager
///
struct GLES3RendererImpl
{
    /// shader
    std::unordered_map<agv::scene::ShaderType, std::shared_ptr<GLES3Shader>> m_shader_map;
    std::shared_ptr<GLES3Shader> GetOrCreateShader(agv::scene::ShaderType shaderType)
    {
        auto found = m_shader_map.find(shaderType);
        if (found != m_shader_map.end())
        {
            return found->second;
        }

        auto shader = GLES3Shader::Create((int)shaderType);
        if (!shader)
        {
            LOGE << "fail to GLES3Shader::Create";
            return nullptr;
        }
        m_shader_map.insert(std::make_pair(shaderType, shader));

        return shader;
    }

    /// texture
    std::unordered_map<uint32_t, std::shared_ptr<GLES3Texture>> m_texture_map;
    std::shared_ptr<GLES3Texture> GetOrCreateTexture(const agv::scene::Texture *pTexture)
    {
        auto found = m_texture_map.find(pTexture->GetID());
        if (found != m_texture_map.end())
        {
            return found->second;
        }

        auto texture = std::make_shared<GLES3Texture>();
        m_texture_map.insert(std::make_pair(pTexture->GetID(), texture));

        StbImage image;
        if (!image.Load(pTexture->Bytes.data, pTexture->Bytes.size))
        {
            LOGE << "fail to load image: " << pTexture->GetName();
            return nullptr;
        }

        texture->SetImage(image.width, image.height, image.channels, (const std::byte *)image.data);

        return texture;
    }

    /// material
    std::unordered_map<uint32_t, std::shared_ptr<GLES3Material>> m_material_map;
    std::shared_ptr<GLES3Material> GetOrCreateMaterial(const agv::scene::Material *pMaterial)
    {
        auto found = m_material_map.find(pMaterial->GetID());
        if (found != m_material_map.end())
        {
            return found->second;
        }

        auto material = std::make_shared<GLES3Material>();
        m_material_map.insert(std::make_pair(pMaterial->GetID(), material));

        material->Shader = GetOrCreateShader(pMaterial->ShaderType);

        // texture
        if (pMaterial->ColorTexture)
        {
            material->Texture = GetOrCreateTexture(&*pMaterial->ColorTexture);
        }

        return material;
    }

    /// vertex buffer
    std::unordered_map<uint32_t, std::shared_ptr<GLES3VertexBufferGroup>> m_vertexBuffer_map;
    std::shared_ptr<GLES3VertexBufferGroup> GetOrCreateVertexBuffer(const agv::scene::Mesh *pMesh)
    {
        {
            auto found = m_vertexBuffer_map.find(pMesh->GetID());
            if (found != m_vertexBuffer_map.end())
            {
                return found->second;
            }
        }

        auto vbo = std::make_shared<GLES3VertexBufferGroup>(pMesh->GetVertexCount(), pMesh->Topology);
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

    std::unordered_map<uint32_t, std::shared_ptr<GLES3VertexArray>> m_vertexArray_map;
    std::shared_ptr<GLES3VertexArray> GetOrCreateVertexArray(
        const agv::scene::Submesh *pSubmesh,
        const std::shared_ptr<GLES3VertexBufferGroup> &vbo,
        const std::shared_ptr<GLES3Shader> &shader)
    {
        {
            auto found = m_vertexArray_map.find(pSubmesh->GetID());
            if (found != m_vertexArray_map.end())
            {
                return found->second;
            }
        }

        auto vao = std::make_shared<GLES3VertexArray>();
        m_vertexArray_map.insert(std::make_pair(pSubmesh->GetID(), vao));

        vao->Bind();

        auto position = vbo->m_attributes["POSITION"];
        {
            position->Bind();
            vao->BindSlot(0, position);
        }

        {
            // auto normal = vbo->m_attributes["NORMAL"];
            // if (normal)
            // {
            //     normal->Bind();
            //     vao->BindSlot(1, normal);
            // }
        }

        auto texcoord = vbo->m_attributes["TEXCOORD_0"];
        if (texcoord)
        {
            texcoord->Bind();
            vao->BindSlot(2, texcoord);
        }

        if (vbo->m_indices)
        {
            vbo->m_indices->Bind();
        }

        vao->Unbind();
        position->Unbind();
        if (texcoord)
        {
            texcoord->Unbind();
        }

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
    auto meshGroup = node->MeshGroup;
    if (meshGroup)
    {
        for (auto &mesh : meshGroup->Meshes)
        {
            auto vbo = m_impl->GetOrCreateVertexBuffer(&*mesh);

            int offset = 0;
            for (auto &submesh : mesh->Submeshes)
            {
                auto material = m_impl->GetOrCreateMaterial(&*submesh->GetMaterial());
                auto shader = material->Shader;
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

                    // set texture
                    material->Set();

                    auto vao = m_impl->GetOrCreateVertexArray(&*submesh, vbo, shader);
                    vao->Bind();
                    vbo->Draw(offset, submesh->GetDrawCount());
                }
                offset += submesh->GetDrawCount();
            }
            GLES3VertexArray::Unbind();
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
    glClearDepthf(1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        DrawModel(camera, cameraNode, pScene->GetModel());
    }
}

void GLES3Renderer::DrawModel(const agv::scene::ICamera *camera, const agv::scene::Node *cameraNode,
                              agv::scene::Model *pModel)
{
    for (auto &node : pModel->Nodes)
    {
        DrawNode(camera, cameraNode, &*node);
    }
}

} // namespace renderer
} // namespace agv