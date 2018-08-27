#include "gles3renderer.h"
#include "shader.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <vector>
#include <plog/Log.h>


GLES3Renderer::GLES3Renderer(const std::string &vs, const std::string &fs)
	: m_vs(vs), m_fs(fs)
{
}

void GLES3Renderer::resize(int w, int h)
{
	LOGD << "resize: " << w << ", " << h;
	m_width = w;
	m_height = h;
}

void GLES3Renderer::update()
{
}

void GLES3Renderer::draw()
{
	if (!m_shader) {
		m_shader = Shader::Create(m_vs, m_fs);
		if (!m_shader) {
			return;
		}

		// Bind vPosition to attribute 0
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f,  0.0f };

	// Set the viewport
	glViewport(0, 0, m_width, m_height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	m_shader->Use();

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
