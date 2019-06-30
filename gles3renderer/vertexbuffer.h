#pragma once
#include <stdint.h>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <simplegltf/simplegltf.h>


namespace agv {
	namespace renderer {
		class VertexBuffer
		{
			bool m_isIndex = false;
			uint32_t m_vbo = 0;

		public:
			VertexBuffer();
			~VertexBuffer();
			void Bind();
			void Unbind();
			void BufferData(bool isIndex, const std::byte *values, size_t byteSize);
		};


		class VertexArray
		{
			uint32_t m_vao = 0;

			std::unordered_map<std::string, std::shared_ptr<VertexBuffer>> m_attributes;

			uint32_t m_topology;
			int m_vertexCount;

			std::shared_ptr<VertexBuffer> m_indices;
			int m_indexCount;
			uint32_t m_indexType;

		public:
			VertexArray(int vertexCount, simplegltf::GltfTopologyType topology);
			VertexArray(int vertexCount) : VertexArray(vertexCount, simplegltf::GltfTopologyType::TRIANGLES)
			{}
			~VertexArray();
			static std::shared_ptr<VertexArray> CreateTriangles(int vertexCount, const float *vertices, const float *colors);
			static std::shared_ptr<VertexArray> CreateLines(int vertexCount, const float *vertices, const float *colors);

			void Bind();
			void Unbind();
			void AddAttribute(const std::string &semantic, const simplegltf::View &view);
			void SetIndex(const simplegltf::View &view);
			void Draw();
		};
	}
}
