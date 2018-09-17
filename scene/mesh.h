#pragma once
#include <string>
#include <vector>
#include "material.h"
#include <glm/glm.hpp>


namespace agv {
	namespace scene {
		class GLTFLoader;
		class Mesh
		{
		public:
			enum Topology
			{
				Triangles,
				Lines,
			};

		private:

			renderer::Material m_material;
			Topology m_topology = Topology::Triangles;
			std::vector<glm::vec3> m_vertices;
			std::vector<glm::vec3> m_colors;

		public:

			static std::shared_ptr<Mesh> CreateGrid(const renderer::Material &material,
				float size, int count);
			static std::shared_ptr<Mesh> CreateAxis(const renderer::Material &material,
				float size);
			static std::shared_ptr<Mesh> CreateSampleTriangle(const renderer::Material &material,
				float size);

			static std::shared_ptr<Mesh> CreateFromGltf(const renderer::Material &material,
				const std::shared_ptr<GLTFLoader> &gltf, int meshIndex);

			const renderer::Material& GetMaterial()const { return m_material; }
			Topology GetTopology()const { return m_topology; }
			int GetVertexCount()const { return static_cast<int>(m_vertices.size()); }
			const float* GetVertices()const { return (const float*)m_vertices.data(); }
			const float* GetColors()const { return (const float*)m_colors.data(); }
		};
	}
}
