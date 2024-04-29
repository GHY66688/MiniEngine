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
		//GL_RGB8定义了OpenGL如何存储数据
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//改成 GL_NEAREST,不会模糊

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);	//重复贴图
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		int width, height, channels;
		//翻转加载，因为OpenGL以左下为起点，而stbi以左上为起点
		stbi_set_flip_vertically_on_load(1);
		//若失效返回空指针
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
		//GL_RGB8定义了OpenGL如何存储数据
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);


		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//改成 GL_NEAREST,不会模糊

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);	//重复贴图
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		//0级，后面两个0是针对该图片的偏移，可以选取部分图片作为纹理 GL_RGB:上传数据的格式
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		//将数据上传到GPU后，便可释放以节约CPU内存
		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}

	//size只是为了安全
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
