#include "vertexbuffer.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <assert.h>


namespace agv {
	namespace renderer {
		///
		/// VertexBuffer
		///
		VertexBuffer::VertexBuffer()
		{
			glGenBuffers(1, &m_vbo);
			assert(m_vbo);
		}

		VertexBuffer::~VertexBuffer()
		{
			glDeleteBuffers(1, &m_vbo);
			m_vbo = 0;
		}

		void VertexBuffer::Bind()
		{
			if (m_isIndex) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo);
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
			}
		}

		void VertexBuffer::Unbind()
		{
			if (m_isIndex) {
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
			else {
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}

		void VertexBuffer::BufferData(bool isIndex, const uint8_t *values, int byteSize)
		{
			m_isIndex = isIndex;

			Bind();
			{
				if (m_isIndex) {
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, values, GL_STATIC_DRAW);
				}
				else {
					glBufferData(GL_ARRAY_BUFFER, byteSize, values, GL_STATIC_DRAW);
				}
				Unbind();
			}
		}

		///
		/// VertexArray
		///
		VertexArray::VertexArray(uint32_t topology, int vertexCount)
			: m_topology(topology), m_vertexCount(vertexCount)
		{
			glGenVertexArrays(1, &m_vao);
			assert(m_vao);
		}

		VertexArray::~VertexArray()
		{
			glDeleteVertexArrays(1, &m_vao);
			m_vao = 0;
		}

		std::shared_ptr<VertexArray> VertexArray::Create(uint32_t topology,
			int vertexCount,
			const float *vertices,
			const float *colors)
		{
			auto vao = std::make_shared<VertexArray>(topology, vertexCount);

			{
				auto vbo = std::make_shared<VertexBuffer>();
				vbo->BufferData(false, (uint8_t*)vertices, vertexCount * 3 * 4);
				vao->AddAttribute(vbo, 3);
			}

			{
				auto vbo = std::make_shared<VertexBuffer>();
				vbo->BufferData(false, (uint8_t*)colors, vertexCount * 3 * 4);
				vao->AddAttribute(vbo, 3);
			}

			return vao;
		}

		std::shared_ptr<VertexArray> VertexArray::CreateTriangles(int vertexCount, const float *vertices, const float *colors)
		{
			return Create(GL_TRIANGLES, vertexCount, vertices, colors);
		}

		std::shared_ptr<VertexArray> VertexArray::CreateLines(int vertexCount, const float *vertices, const float *colors)
		{
			return Create(GL_LINES, vertexCount, vertices, colors);
		}

		void VertexArray::Bind()
		{
			glBindVertexArray(m_vao);
		}

		void VertexArray::Unbind()
		{
			glBindVertexArray(0);
		}

		void VertexArray::AddAttribute(const std::shared_ptr<VertexBuffer> &vbo, int components)
		{
			auto attribute = static_cast<GLuint>(m_attributes.size());
			m_attributes.push_back(vbo);

			Bind();
			{
				vbo->Bind();
				glEnableVertexAttribArray(attribute);
				glVertexAttribPointer(attribute, components, GL_FLOAT, GL_FALSE, 0, nullptr);
				Unbind();
			}

			glDisableVertexAttribArray(attribute);
			vbo->Unbind();
		}

		void VertexArray::SetIndex(const std::shared_ptr<VertexBuffer> &vbo, int indexCount, uint32_t indexType)
		{
			m_indices = vbo;
			m_indexCount = indexCount;
			m_indexType = indexType;

			Bind();
			{
				vbo->Bind();
				Unbind();
			}
			vbo->Unbind();
		}

		void VertexArray::Draw()
		{
			Bind();

			if (m_indices) {
				glDrawElements(m_topology, m_indexCount, m_indexType, 0);
			}
			else {
				glDrawArrays(m_topology, 0, m_vertexCount);
			}
		}
	}
}
