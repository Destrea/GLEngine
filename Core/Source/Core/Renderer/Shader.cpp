#include "Shader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
namespace Renderer {

	Shader &Shader::Use()
	{
		glUseProgram(this->ID);
		return *this;
	}

	static std::string ReadTextFile(const std::filesystem::path& path)
	{
		std::ifstream file(path);

		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << path.string() << std::endl;
			return {};
		}

		std::ostringstream contentStream;
		contentStream << file.rdbuf();
		return contentStream.str();
	}

	uint32_t CreateComputeShader(const std::filesystem::path& path)
	{
		std::string shaderSource = ReadTextFile(path);

		GLuint shaderHandle = glCreateShader(GL_COMPUTE_SHADER);

		const GLchar* source = (const GLchar*)shaderSource.c_str();
		glShaderSource(shaderHandle, 1, &source, 0);

		glCompileShader(shaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(shaderHandle);
			return -1;
		}

		GLuint program = glCreateProgram();
		glAttachShader(program, shaderHandle);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(program);
			glDeleteShader(shaderHandle);

			return -1;
		}

		glDetachShader(program, shaderHandle);
		return program;
	}

	uint32_t ReloadComputeShader(uint32_t shaderHandle, const std::filesystem::path& path)
	{
		uint32_t newShaderHandle = CreateComputeShader(path);

		// Return old shader if compilation failed
		if (newShaderHandle == -1)
			return shaderHandle;

		glDeleteProgram(shaderHandle);
		return newShaderHandle;
	}

	uint32_t CreateGraphicsShader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		std::string vertexShaderSource = ReadTextFile(vertexPath);
		std::string fragmentShaderSource = ReadTextFile(fragmentPath);

		// Vertex shader

		GLuint vertexShaderHandle = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* source = (const GLchar*)vertexShaderSource.c_str();
		glShaderSource(vertexShaderHandle, 1, &source, 0);

		glCompileShader(vertexShaderHandle);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(vertexShaderHandle);
			return -1;
		}

		// Fragment shader

		GLuint fragmentShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);

		source = (const GLchar*)fragmentShaderSource.c_str();
		glShaderSource(fragmentShaderHandle, 1, &source, 0);

		glCompileShader(fragmentShaderHandle);

		isCompiled = 0;
		glGetShaderiv(fragmentShaderHandle, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShaderHandle, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShaderHandle, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteShader(fragmentShaderHandle);
			return -1;
		}

		// Program linking

		GLuint program = glCreateProgram();
		glAttachShader(program, vertexShaderHandle);
		glAttachShader(program, fragmentShaderHandle);
		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			std::cerr << infoLog.data() << std::endl;

			glDeleteProgram(program);
			glDeleteShader(vertexShaderHandle);
			glDeleteShader(fragmentShaderHandle);

			return -1;
		}

		glDetachShader(program, vertexShaderHandle);
		glDetachShader(program, fragmentShaderHandle);
		return program;
	}

	uint32_t ReloadGraphicsShader(uint32_t shaderHandle, const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath)
	{
		uint32_t newShaderHandle = CreateGraphicsShader(vertexPath, fragmentPath);

		// Return old shader if compilation failed
		if (newShaderHandle == -1)
			return shaderHandle;

		glDeleteProgram(shaderHandle);
		return newShaderHandle;
	}



	void Shader::Compile(const char* vertexSource, const char* fragmentSource)
	{
		unsigned int sVertex, sFragment, gShader;
		//Vertex Shader
		sVertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(sVertex, 1, &vertexSource, NULL);
		glCompileShader(sVertex);
		checkCompileErrors(sVertex, "VERTEX");
		//Fragment Shader
		sFragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(sFragment, 1, &fragmentSource, NULL);
		glCompileShader(sFragment);
		checkCompileErrors(sFragment, "FRAGMENT");
		/* optional geometry shader source compile check
		 *   if (geometrySource != nullptr)
		 *   {
		 *       gShader = glCreateShader(GL_GEOMETRY_SHADER);
		 *       glShaderSource(gShader, 1, &geometrySource, NULL);
		 *       glCompileShader(gShader);
		 *       checkCompileErrors(gShader, "GEOMETRY");
	}*/
		//Shader Program
		this->ID = glCreateProgram();
		glAttachShader(this->ID, sVertex);
		glAttachShader(this->ID, sFragment);
		//if(geometrySource != nullptr)
		//    glAttachShader(this->ID, gShader);
		glLinkProgram(this->ID);
		checkCompileErrors(this->ID, "PROGRAM");
		// delete shaders after linking for cleanup
		glDeleteShader(sVertex);
		glDeleteShader(sFragment);
		//if(geometrySource != nullptr)
		//  glDeleteShader(gShader);
	}


	//utility functions
	void Shader::setFloat(const char *name, float value, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform1f(glGetUniformLocation(this->ID, name), value);
	}
	void Shader::setInteger (const char *name, int value, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform1f(glGetUniformLocation(this->ID, name), value);
	}
	void Shader::setVector2f(const char *name, float x, float y, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->ID, name), x, y);
	}
	void Shader::setVector2f(const char *name, const glm::vec2 &value, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
	}

	void Shader::setVector3f(const char *name, float x, float y, float z, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
	}
	void Shader::setVector3f(const char *name, const glm::vec3 &value, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
	}
	void Shader::setVector4f(const char *name, float x, float y, float z, float w, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
	}
	void Shader::setVector4f(const char *name, const glm::vec4 &value, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
	}
	void Shader::setMatrix4 (const char *name, const glm::mat4 &matrix, bool useShader)
	{
		if(useShader)
			this->Use();
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, false, glm::value_ptr(matrix));
	}

	void Shader::checkCompileErrors(unsigned int object, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(object, GL_COMPILE_STATUS, &success);
			if(!success)
			{
				glGetShaderInfoLog(object, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			GLint isLinked = 0;
			glGetProgramiv(object, GL_LINK_STATUS, &success);
			if(!success)
			{
				glGetProgramiv(object, GL_LINK_STATUS, (int*)&isLinked);
				if (isLinked == GL_FALSE)
				{
					GLint maxLength = 0;
					glGetProgramiv(object, GL_INFO_LOG_LENGTH, &maxLength);

					std::vector<GLchar> infoLog(maxLength);
					glGetProgramInfoLog(object, maxLength, &maxLength, &infoLog[0]);

					std::cerr << infoLog.data() << std::endl;

					glDeleteProgram(object);


				}
			}
		}
	}

}
