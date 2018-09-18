#pragma once
#include <stdint.h>
#include <vector>
#include <memory>


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
			void BufferData(bool isIndex, const uint8_t *values, int byteSize);
		};


		class VertexArray
		{
			uint32_t m_vao = 0;

			std::vector<std::shared_ptr<VertexBuffer>> m_attributes;

			uint32_t m_topology;
			int m_vertexCount;

			std::shared_ptr<VertexBuffer> m_indices;
			int m_indexCount;
			uint32_t m_indexType;

		public:
			VertexArray(uint32_t topology, int vertexCount);
			~VertexArray();
			static std::shared_ptr<VertexArray> Create(uint32_t topology, int vertexCount,
				const float *vertices,
				const float *colors);
			static std::shared_ptr<VertexArray> CreateTriangles(int vertexCount, const float *vertices, const float *colors);
			static std::shared_ptr<VertexArray> CreateLines(int vertexCount, const float *vertices, const float *colors);

			void Bind();
			void Unbind();
			void AddAttribute(const std::shared_ptr<VertexBuffer> &vbo, int components);
			void SetIndex(const std::shared_ptr<VertexBuffer> &vbo, int indexCount, uint32_t indexType);
			void Draw();
		};
	}
}
