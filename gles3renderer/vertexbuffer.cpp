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
			glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		}

		void VertexBuffer::BufferData(const float *values, int count)
		{
			Bind();
			glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), values, GL_STATIC_DRAW);
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
			auto vao = std::make_shared<VertexArray>(topology, vertexCount/3);

			{
				auto vbo = std::make_shared<VertexBuffer>();
				vbo->BufferData(vertices, vertexCount*3);
				vao->AddAttribute(vbo, 3);
			}

			{
				auto vbo = std::make_shared<VertexBuffer>();
				vbo->BufferData(colors, vertexCount * 3);
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

		void VertexArray::AddAttribute(const std::shared_ptr<VertexBuffer> &vbo, int components)
		{
			Bind();
			auto attribute = static_cast<GLuint>(m_attributes.size());
			m_attributes.push_back(vbo);

			glEnableVertexAttribArray(attribute);
			vbo->Bind();
			glVertexAttribPointer(attribute, components, GL_FLOAT, GL_FALSE, 0, nullptr);
		}

		void VertexArray::Draw()
		{
			Bind();
			glDrawArrays(m_topology, 0, m_vertexCount);
		}
	}
}