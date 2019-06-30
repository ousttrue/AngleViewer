#pragma once
#include <string>
#include <vector>
#include <memory>
#include "material.h"
#include <simplegltf/simplegltf.h>


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
			simplegltf::View m_vertices;
			simplegltf::View m_colors;
			simplegltf::View m_indices;

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
			const simplegltf::View& GetVertices()const { return m_vertices; }
			const simplegltf::View& GetColors()const { return m_colors; }
			const simplegltf::View& GetIndices()const { return m_indices; }
		};
	}
}
