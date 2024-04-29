#include "MGpch.h"


#include "OpenGLTexture.h"

#include <stb_image.h>


namespace MG {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		:m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//GL_RGB8������OpenGL��δ洢����
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//�ĳ� GL_NEAREST,����ģ��

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);	//�ظ���ͼ
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		//��ת���أ���ΪOpenGL������Ϊ��㣬��stbi������Ϊ���
		stbi_set_flip_vertically_on_load(1);
		//��ʧЧ���ؿ�ָ��
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		MG_CORE_ASSERT(data, "Failed to load image!  {0}", path);
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		MG_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not support!");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		//GL_RGB8������OpenGL��δ洢����
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//�ĳ� GL_NEAREST,����ģ��

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);	//�ظ���ͼ
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//0������������0����Ը�ͼƬ��ƫ�ƣ�����ѡȡ����ͼƬ��Ϊ���� GL_RGB:�ϴ����ݵĸ�ʽ
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		//�������ϴ���GPU�󣬱���ͷ��Խ�ԼCPU�ڴ�
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	//sizeֻ��Ϊ�˰�ȫ
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;	//bpp bytes per pixel GL_RGBA = 4 GL_RGB = 3
		MG_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}	//end MG
