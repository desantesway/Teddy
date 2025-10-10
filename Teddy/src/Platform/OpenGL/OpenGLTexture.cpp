#include "TeddyPch.h"
#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL/OpenGLTextureFormats.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Teddy 
{

	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification)
		: m_Specification(specification), m_Width(m_Specification.Width), m_Height(m_Specification.Height)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		m_Width = specification.Width;
		m_Height = specification.Height;

		m_InternalFormat = Utils::TeddyTextureFormatToGLInternalFormat(m_Specification.Format);
		m_DataFormat = Utils::TeddyTextureFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		m_FilterFormat = Utils::TeddyTextureFilterFormatToGL(m_Specification.Filter);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_FilterFormat);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_FilterFormat);

		m_WrapFormat = Utils::TeddyTextureWrapFormatToGL(m_Specification.Wrap);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_WrapFormat);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_WrapFormat);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, const TextureSpecification& specification)
		: m_Path(path)
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		int width, height, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc* data = nullptr;

		{
			TED_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			m_IsLoaded = true;

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

			TED_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			// Pre-multiply alpha
			if (channels == 4)
			{
				for (int i = 0; i < width * height; ++i)
				{
					unsigned char* pixel = &data[i * 4];

					float r = pixel[0] / 255.0f;
					float g = pixel[1] / 255.0f;
					float b = pixel[2] / 255.0f;
					float a = pixel[3] / 255.0f;

					r *= a;
					g *= a;
					b *= a;

					pixel[0] = static_cast<unsigned char>(r * 255.0f);
					pixel[1] = static_cast<unsigned char>(g * 255.0f);
					pixel[2] = static_cast<unsigned char>(b * 255.0f);
				}
			}

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

			m_FilterFormat = Utils::TeddyTextureFilterFormatToGL(specification.Filter);
			m_MinFilterFormat = Utils::TeddyTextureFilterFormatToGL(specification.MinFilter);

			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_MinFilterFormat);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_FilterFormat);

			m_WrapFormat = Utils::TeddyTextureWrapFormatToGL(specification.Wrap);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_WrapFormat);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_WrapFormat);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);
			
			// Enable this for colored mip map
			if (false)
				GenerateColoredMipMap();
			else
				glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::GenerateColoredMipMap()
	{
		int levels = 1 + (int)std::floor(std::log2(std::max(m_Width, m_Height)));

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, levels, m_InternalFormat, m_Width, m_Height);

		for (int level = 1; level < levels; ++level)
		{
			int mipWidth = std::max(1, (int)(m_Width >> level));
			int mipHeight = std::max(1, (int)(m_Height >> level));

			std::vector<unsigned char> mipData(mipWidth * mipHeight * 4);

			glm::u8vec4 color;
			switch (level % 6)
			{
			case 0: color = { 255,   0,   0, 255 }; break; 
			case 1: color = { 0, 255,   0, 255 }; break; 
			case 2: color = { 0,   0, 255, 255 }; break; 
			case 3: color = { 255, 255,   0, 255 }; break; 
			case 4: color = { 255,   0, 255, 255 }; break; 
			case 5: color = { 0, 255, 255, 255 }; break; 
			}

			for (int i = 0; i < mipWidth * mipHeight; i++)
			{
				mipData[i * 4 + 0] = color.r;
				mipData[i * 4 + 1] = color.g;
				mipData[i * 4 + 2] = color.b;
				mipData[i * 4 + 3] = color.a;
			}

			// Upload mip level
			glTextureSubImage2D(m_RendererID, level, 0, 0, mipWidth, mipHeight,
				GL_RGBA, GL_UNSIGNED_BYTE, mipData.data());
		}
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) 
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		TED_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		TED_PROFILE_CAT(InstrumentorCategory::Streaming);

		glActiveTexture(GL_TEXTURE0);
		glBindTextureUnit(slot, m_RendererID);
	}

}