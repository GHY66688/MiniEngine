#pragma once

#include <string>
#include <glm/glm.hpp>

#include "MiniEngine/Renderer/Shader.h"

//���ڸ�ͷ�ļ���Ҫ��SandBoxAPP���ã���SandBoxAPP��û�з���glad��Ȩ�ޣ������޷��ڸ�ͷ�ļ��а���glad������GLenum
//TODO:REMOVE!
typedef unsigned int GLenum;

namespace MG {

	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const;
		virtual void Unbind() const;

		virtual const std::string& GetName() const override { return m_Name; }
		virtual void SetName(const std::string& name) override { m_Name = name; }



		void UploadUniformInt(const std::string& name, int value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Compile(std::unordered_map<GLenum, std::string>& shaderSources);

	private:
		uint32_t m_RendererID;
		std::string m_Name;
	};

}	//end MG