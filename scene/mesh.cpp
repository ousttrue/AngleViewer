#include "mesh.h"
#include "loader.h"
#include <plog/Log.h>


namespace agv {
	namespace scene {
		std::shared_ptr<Mesh> Mesh::CreateGrid(const renderer::Material &material, float grid_size, int grid_count)
		{
			auto grid_edge = grid_size * grid_count;
			std::vector<glm::vec3> vertices;
			std::vector<glm::vec3> colors;
			for (int i = -grid_count; i <= grid_count; ++i)
			{
				if (i == 0)continue;

				vertices.push_back(glm::vec3(-grid_edge, 0, grid_size*i));
				colors.push_back(glm::vec3(0.5f, 0.5, 0.5f));

				vertices.push_back(glm::vec3(grid_edge, 0, grid_size*i));
				colors.push_back(glm::vec3(0.5f, 0.5, 0.5f));
			}
			for (int i = -grid_count; i <= grid_count; ++i)
			{
				if (i == 0)continue;

				vertices.push_back(glm::vec3(grid_size*i, 0, -grid_edge));
				colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));

				vertices.push_back(glm::vec3(grid_size*i, 0, grid_edge));
				colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
			}

			auto mesh = std::make_shared<Mesh>();
			mesh->m_material = material;
			mesh->m_topology = Mesh::Lines;
			mesh->m_vertices = vertices;
			mesh->m_colors = colors;
			return mesh;
		}

		std::shared_ptr<Mesh> Mesh::CreateAxis(const renderer::Material &material, float grid_edge)
		{
			std::vector<glm::vec3> vertices = {
				// x
				{-grid_edge, 0, 0},
				{0, 0, 0},

				{0, 0, 0},
				{grid_edge, 0, 0},
				// y
			{0, -grid_edge, 0},
			{0, 0, 0},

			{0, 0, 0},
			{0, grid_edge, 0},
			// z
		{0, 0, -grid_edge},
		{0, 0, 0},

		{0, 0, 0},
		{0, 0, grid_edge},
			};

			std::vector<glm::vec3> colors = {
				// red
				{0.5f, 0, 0},
				{0.5f, 0, 0},
				{1.0f, 0, 0},
				{1.0f, 0, 0},
				// green
				{0, 0.5f, 0},
				{0, 0.5f, 0},
				{0, 1.0f, 0},
				{0, 1.0f, 0},
				// blue
				{0, 0, 0.5f},
				{0, 0, 0.5f},
				{0, 0, 1.0f},
				{0, 0, 1.0f},
			};

			auto mesh = std::make_shared<Mesh>();
			mesh->m_material = material;
			mesh->m_topology = Mesh::Lines;
			mesh->m_vertices = vertices;
			mesh->m_colors = colors;
			return mesh;
		}

		std::shared_ptr<Mesh> Mesh::CreateSampleTriangle(const renderer::Material &material, float size)
		{
			std::vector<glm::vec3> vertices = {
				{0.0f,  size, 0.0f},
				{-size, -size, 0.0f},
				{size, -size,  0.0f} };

			std::vector<glm::vec3> colors = {
				{1.0f, 0, 0},
				{0, 1.0f, 0},
				{0, 0, 1.0f},
			};

			auto mesh = std::make_shared<Mesh>();
			mesh->m_material = material;
			mesh->m_vertices = vertices;
			mesh->m_colors = colors;
			return mesh;
		}

		std::shared_ptr<Mesh> Mesh::CreateFromGltf(const renderer::Material &material,
			const std::shared_ptr<GLTFLoader> &gltf, int meshIndex)
		{
			auto mesh = std::make_shared<Mesh>();
			mesh->m_material = material;

			auto primitiveCount = gltf->MeshGetPrimitiveCount(meshIndex);
			for (int i = 0; i < primitiveCount; ++i)
			{
				auto values= gltf->MeshReadPrimitiveAttribute(meshIndex, i, "POSITION");
				if (!values.empty()) {
					mesh->m_vertices.assign((glm::vec3*)values.data(), (glm::vec3*)(values.data() + values.size()));
				}
				//mesh->m_colors = gltf->MeshReadPrimitiveAttribute(meshIndex, i, "COLOR_0");
				if (mesh->m_colors.size() != mesh->m_vertices.size())
				{
					mesh->m_colors.resize(mesh->m_vertices.size(), glm::vec3(1.0f));
				}

				return mesh;
			}

			return nullptr;
		}
	}
}
