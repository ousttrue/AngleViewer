#include "glrenderer.h"
#include <GLES2/gl2.h> 
#include <vector>
#include <plog/Log.h>


GLuint LoadShader(GLenum type, const char *shaderSrc)
{
	LOGD << "LoadShader";

	// Create the shader object
	auto shader = glCreateShader(type);
	if (shader == 0) {
		return 0;
	}

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, nullptr);

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
		if (infoLen > 1)
		{
			std::vector<char> infoLog(infoLen);
			glGetShaderInfoLog(shader, infoLen, NULL, infoLog.data());
			LOGE << "Error compiling shader: " << infoLog.data();
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}


bool GlRenderer::initilize()
{
	//UserData *userData = esContext->userData;
	auto vShaderStr =
		"attribute vec4 vPosition;   \n"
		"void main()                 \n"
		"{                           \n"
		"  gl_Position = vPosition;  \n"
		"}                           \n"
		;

	auto fShaderStr =
		"precision mediump float;                   \n"
		"void main()                                \n"
		"{                                          \n"
		"  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
		"}                                          \n"
		;

	// Load the vertex/fragment shaders
	auto vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
	auto fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

	// Create the program object
	m_programObject = glCreateProgram();
	if (m_programObject == 0) {
		LOGE << "fail to glCreateProgram";
		return false;
	}

	glAttachShader(m_programObject, vertexShader);
	glAttachShader(m_programObject, fragmentShader);

	// Bind vPosition to attribute 0
	glBindAttribLocation(m_programObject, 0, "vPosition");

	// Link the program
	glLinkProgram(m_programObject);

	// Check the link status
	GLint linked;
	glGetProgramiv(m_programObject, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(m_programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			std::vector<char> infoLog(infoLen);

			glGetProgramInfoLog(m_programObject, infoLen, NULL, infoLog.data());
			LOGE << "Error linking program: " << infoLog.data();
		}

		glDeleteProgram(m_programObject);
		return false;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	return true;
}

void GlRenderer::resize(int w, int h)
{
	LOGD << "resize: " << w << ", " << h;
	m_width = w;
	m_height = h;
}

void GlRenderer::update()
{
}

void GlRenderer::draw()
{
	if (!m_initialized) {
		if (!initilize()) {
			return;
		}
		m_initialized = true;
	}

	GLfloat vVertices[] = { 0.0f,  0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f,  0.0f };

	// Set the viewport
	glViewport(0, 0, m_width, m_height);

	// Clear the color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the program object
	glUseProgram(m_programObject);

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}
