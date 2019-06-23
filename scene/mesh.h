#pragma once
#include <string>
#include <vector>
#include <memory>
#include "material.h"
#include "bytebuffer.h"


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
			ByteBuffer m_vertices;
			ByteBuffer m_colors;
			ByteBuffer m_indices;

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
			const ByteBuffer& GetVertices()const { return m_vertices; }
			const ByteBuffer& GetColors()const { return m_colors; }
			const ByteBuffer& GetIndices()const { return m_indices; }
		};
	}
}
