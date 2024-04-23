#include "MGpch.h"
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>

namespace MG {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
		{
			return GL_VERTEX_SHADER;
		}
		if (type == "fragment" || type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}
		MG_CORE_ASSERT(false, "Unknown shader type : {0}", type);
		return 0;
	}


	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);
	}

	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		shaderSources[GL_VERTEX_SHADER] = vertexSrc;
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(shaderSources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		//使用unordered_map缓存location，避免对相同name进行重复定位
		//-1则代表没有找到
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		//uniform位置， 几个矩阵， 是否转置(OpenGL和glm都主列，无需转置，若DirectX和glm，则需要转置，因为DirectX主行)
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		//-1则代表没有找到
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		//uniform位置， 几个矩阵， 是否转置(OpenGL和glm都主列，无需转置，若DirectX和glm，则需要转置，因为DirectX主行)
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in, std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);	//文件指针到达文件末尾
			result.resize(in.tellg());	//得知文件大小
			in.seekg(0, std::ios::beg);	//到达文件头部
			in.read(&result[0], result.size());	//读入数据到result
			in.close();
		}
		else
		{
			MG_CORE_ERROR("Could not open file: {0}", filepath);
		}
		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;
		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);	//找到该行末尾
			MG_CORE_ASSERT(eol != std::string::npos, "Syntax error");

			size_t begin = pos + typeTokenLength + 1;	//+1 是为了把#type后面的空格去掉
			std::string type = source.substr(begin, eol - begin);
			MG_CORE_ASSERT(ShaderTypeFromString(type), "Invaild shader type");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);	//下一行开始即#version那一行
			pos = source.find(typeToken, nextLinePos);	//找到下一个#type,若没有返回std::string::npos

			//读取shader，如果nextLinePos == std::string::npos，那么pos也等于std::string::npos
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}
		return shaderSources;
	}

	void OpenGLShader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		GLuint program = glCreateProgram();
		std::vector<GLenum> glShaderIDs(shaderSources.size());
		for (const auto& [key, value] : shaderSources)
		{
			GLuint shader = glCreateShader(key);
			const GLchar* source = (const GLchar*)value.c_str();
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				MG_CORE_ERROR("{0}", infoLog.data());
				MG_CORE_ASSERT(false, "{0} Shader Compile Failed!", key);
				break;
			}
			glAttachShader(program, shader);
			glShaderIDs.push_back(shader);
		}
		

		//可以使得shader全部读取成功后 设置program
		m_RendererID = program;



		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			for (auto& id : glShaderIDs)
			{
				glDeleteShader(id);
			}

			// Use the infoLog as you see fit.
			MG_CORE_ERROR("{0}", infoLog.data());
			MG_CORE_ASSERT(false, "Shader Link Failed!");
			return;
		}

		// Always detach shaders after a successful link.

		for (auto& id : glShaderIDs)
		{
			glDetachShader(program, id);
		}

		m_RendererID = program;
	}



}	//end MG