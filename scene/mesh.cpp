#include "mesh.h"
#include <GLTFSDK/Document.h>
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

		template<typename T>
		static std::vector<T> ReadAccessor(const std::shared_ptr<Microsoft::glTF::Document> &gltf, int accessorIndex)
		{
			auto &accessor = gltf->accessors[accessorIndex];
			auto viewIndex = atoi(accessor.bufferViewId.c_str());
			auto &view = gltf->bufferViews[viewIndex];
			auto bufferIndex = atoi(view.bufferId.c_str());
			auto &buffer = gltf->buffers[bufferIndex];

			std::vector<T> values;
			return values;
		}

		std::shared_ptr<Mesh> Mesh::CreateFromGltf(const renderer::Material &material, 
			const std::shared_ptr<Microsoft::glTF::Document> &gltf, int meshIndex)
		{
			auto mesh = std::make_shared<Mesh>();
			mesh->m_material = material;

			auto &gltfMesh = gltf->meshes[meshIndex];
			for (auto &primitive : gltfMesh.primitives)
			{
				// cleanup

				// read
				for (auto &kv : primitive.attributes)
				{
					if (kv.first == "POSITION")
					{
						auto index = atoi(kv.second.c_str());
						mesh->m_vertices = ReadAccessor<glm::vec3>(gltf, index);
						mesh->m_colors.resize(mesh->m_vertices.size(), glm::vec3(1.0f));
					}
					else {
						LOGD << "unknown attribute: " << kv.first;
					}
				}

			}

			return nullptr;
		}
	}
}
