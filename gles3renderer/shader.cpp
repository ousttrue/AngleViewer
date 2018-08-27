#include "shader.h"
#define GL_GLEXT_PROTOTYPES
#include <GLES3/gl3.h> 
#include <plog/Log.h>


static GLuint LoadShader(GLenum type, const char *shaderSrc)
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


Shader::Shader(GLuint program) :m_program(program) {
}

std::shared_ptr<Shader> Shader::Create(const std::string &vs, const std::string &fs)
{
    auto vertexShader = LoadShader(GL_VERTEX_SHADER, vs.c_str());
    if (!vertexShader) {
        return  nullptr;
    }
    auto fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fs.c_str());
    if (!fragmentShader) {
        return nullptr;
    }

    // Create the program object
    auto program = glCreateProgram();
    if (!program) {
        LOGE << "fail to glCreateProgram";
        return nullptr;
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glBindAttribLocation(program, 0, "vPosition");

    // Link the program
    glLinkProgram(program);
    // Check the link status
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1)
        {
            std::vector<char> infoLog(infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog.data());
            LOGE << "Error linking program: " << infoLog.data();
        }

        glDeleteProgram(program);
        return nullptr;
    }

    return std::make_shared<Shader>(program);
}

void Shader::Use()
{
    glUseProgram(m_program);
}
