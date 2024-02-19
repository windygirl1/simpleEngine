#include "shaderProgram.hpp"
#include "simpleEngineCore/log.hpp"
#include <glad/glad.h>

namespace SimpleEngine {
	bool createShader(const char* src, const GLenum shaderType, GLuint& shaderId) {
		shaderId = glCreateShader(shaderType);
		glShaderSource(shaderId, 1, &src, nullptr);
		glCompileShader(shaderId);

		GLint success;

		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			char infoLog[1024];
			glGetShaderInfoLog(shaderId, 1024, nullptr, infoLog);

			LOG_CRIT("Shader compilation error:\n{0}", infoLog);
			return false;
		}
		return true;
	}

	ShaderProgram::ShaderProgram(const char* vertexShaderSrc, const char* fragmentShaderSrc) {
		GLuint vertexShaderId = 0;
		if (!createShader(vertexShaderSrc, GL_VERTEX_SHADER, vertexShaderId)) {
			LOG_CRIT("VERTEX_SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			return;
		}

		GLuint fragmentShaderId = 0;
		if (!createShader(fragmentShaderSrc, GL_FRAGMENT_SHADER, fragmentShaderId)) {
			LOG_CRIT("FRAGMENT_SHADER: compile-time error!");
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			return;
		}

		m_id = glCreateProgram();
		glAttachShader(m_id, vertexShaderId);
		glAttachShader(m_id, fragmentShaderId);
		glLinkProgram(m_id);

		GLint success;

		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (success == GL_FALSE)
		{
			GLchar infoLog[1024];
			glGetProgramInfoLog(m_id, 1024, nullptr, infoLog);
			LOG_CRIT("SHADER PROGRAM: Link-time error:\n{0}", infoLog);
			glDeleteProgram(m_id);
			m_id = 0;
			glDeleteShader(vertexShaderId);
			glDeleteShader(fragmentShaderId);
			return;
		}
		else {
			m_isCompiled = true;
		}

		glDetachShader(m_id, vertexShaderId);
		glDetachShader(m_id, fragmentShaderId);
		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(m_id);
	}

	void ShaderProgram::bind() const {
		glUseProgram(m_id);
	}

	void ShaderProgram::unbind() {
		glUseProgram(0);
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) {
		glDeleteProgram(m_id);
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
		return *this;
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) {
		m_id = shaderProgram.m_id;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_id = 0;
		shaderProgram.m_isCompiled = false;
	}
}