#include "Texture.h"
#include "../Utilities/opengl_common.h"

GLuint BufferTexture3D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint depth, GLint pixelFormat, GLenum type, unsigned char* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_3D, textureId );
	
	//glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( GL_TEXTURE_3D, (*iter).first, (*iter).second );
	}
	glTexImage3D( GL_TEXTURE_3D, levelOfDetail, internalFormat, width, height, depth, 0, pixelFormat, type, (GLvoid*)data );
	if ( generateMipMap )
	{
		glGenerateMipmap( GL_TEXTURE_3D );
		CheckGlError("glGenerateMipMap 3D");
	}
	
	return textureId;
}

GLuint BufferTexture2D( GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum dataType, unsigned char* data, bool filterNearest, bool generateMipMap, bool MipMapFilterNearest )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_2D, textureId );
	
	FilterType magFilter = (filterNearest) ? FilterType::NEAREST : FilterType::LINEAR;
	FilterType minFilter;
	if ( generateMipMap )
	{
		minFilter = (filterNearest) ?
		( (MipMapFilterNearest) ? FilterType::NEAREST_MIPMAP_NEAREST : FilterType::NEAREST_MIPMAP_LINEAR ) :
		( (MipMapFilterNearest) ? FilterType::LINEAR_MIPMAP_NEAREST : FilterType::LINEAR_MIPMAP_LINEAR );
	}
	else
	{
		minFilter = (filterNearest) ? FilterType::NEAREST : FilterType::LINEAR;
	}
	
	SetTextureFilter( textureId, GL_TEXTURE_2D, minFilter, magFilter );
	SetTextureWrapping( textureId, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapType::REPEAT );
	SetTextureWrapping( textureId, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapType::REPEAT );

	// target, level of detail, internal format(RGBA), width, height, border(must be 0), pixel format(RGBA), type, data pointer
	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, width, height, 0, pixelFormat, dataType, (GLvoid*)data );
	CheckGlError("glTexImage2D 2D");
	if ( generateMipMap )
	{
		glGenerateMipmap( GL_TEXTURE_2D );
		CheckGlError("glGenerateMipMap 2D");
	}
	
	return textureId;
}

GLuint BufferTexture1D( GLint internalFormat, GLint width, GLint pixelFormat, GLenum dataType, GLvoid* data, bool filterNearest, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_1D, textureId );
	
	FilterType filter = filterNearest ? FilterType::NEAREST : FilterType::LINEAR;
	SetTextureFilter( textureId, GL_TEXTURE_1D, filter, filter );
	
	glTexImage1D( GL_TEXTURE_1D, 0, internalFormat, width, 0, pixelFormat, dataType, data );
	CheckGlError("glTexImage2D 1D");
	if ( generateMipMap )
	{
		glGenerateMipmap( GL_TEXTURE_1D );
		CheckGlError("glGenerateMipMap 1D");
	}
	
	return textureId;
}

void BufferTextureCubeMap( GLuint mapId, GLenum sideTarget, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, unsigned char* data )
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, mapId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GetFilterParameter( FilterType::LINEAR ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GetFilterParameter( FilterType::LINEAR ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetWrapParameter( WrapType::CLAMP_EDGE ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetWrapParameter( WrapType::CLAMP_EDGE ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GetWrapParameter( WrapType::CLAMP_EDGE ) );
	glTexImage2D( sideTarget, 0, internalFormat, width, height, 0, pixelFormat, type, (GLvoid*)data );
	CheckGlError("glTexImage2D CUBE_MAP");
}

void UpdateBufferImage1D( GLuint texture, GLint offX, GLint offY, GLint w, GLint h, GLenum pixelFormat, GLenum dataType, unsigned char* data )
{
	glBindTexture( GL_TEXTURE_1D, texture );
	glTexSubImage1D( GL_TEXTURE_1D, 0, offX, w, pixelFormat, dataType, data );
}

void UpdateBufferImage2D( GLuint texture, GLint offX, GLint offY, GLint w, GLint h, GLenum pixelFormat, GLenum dataType, unsigned char* data, bool mipmap )
{
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexSubImage2D( GL_TEXTURE_2D, 0, offX, offY, w, h, pixelFormat, dataType, data );
	if ( mipmap )
	{
		glGenerateMipmap( GL_TEXTURE_2D );
	}
}

void SetTextureFilter( GLuint texture, GLenum target, FilterType minFilter, FilterType magFilter )
{
	glBindTexture( target, texture );
	glTexParameteri( target, GL_TEXTURE_MIN_FILTER, GetFilterParameter( minFilter ) );
	glTexParameteri( target, GL_TEXTURE_MAG_FILTER, GetFilterParameter( magFilter ) );
}

void SetTextureWrapping( GLuint texture, GLenum target, GLenum wrap, WrapType type )
{
	glBindTexture( target, texture );
	glTexParameteri( target, wrap, GetWrapParameter( type ) );
}

void BindTexture( GLenum textureUnit, GLenum target, GLuint program )
{
	glActiveTexture( textureUnit );
	glBindTexture( target, program );
}

void UnbindTexture( GLenum target )
{
	glBindTexture( target, NULL );
}

void DeleteTexture( GLuint textureId )
{
	glDeleteTextures( 1, &textureId );
}

GLenum GetFilterParameter( FilterType type )
{
	GLenum glType = 0;
	switch (type)
	{
		case FilterType::NEAREST:
		{
			glType = GL_NEAREST;
			break;
		}
		case FilterType::LINEAR:
		{
			glType = GL_LINEAR;
			break;
		}
		case FilterType::NEAREST_MIPMAP_NEAREST:
		{
			glType = GL_NEAREST_MIPMAP_NEAREST;
			break;
		}
		case FilterType::LINEAR_MIPMAP_NEAREST:
		{
			glType = GL_LINEAR_MIPMAP_NEAREST;
			break;
		}
		case FilterType::NEAREST_MIPMAP_LINEAR:
		{
			glType = GL_NEAREST_MIPMAP_LINEAR;
			break;
		}
		case FilterType::LINEAR_MIPMAP_LINEAR:
		{
			glType = GL_LINEAR_MIPMAP_LINEAR;
			break;
		}
		default:
		{
			break;
		}
	}
	return glType;
}

GLenum GetWrapParameter( WrapType type )
{
	GLenum glType = 0;
	switch (type)
	{
		case WrapType::CLAMP_EDGE:
		{
			glType = GL_CLAMP_TO_EDGE;
			break;
		}
		case WrapType::MIRROR_CLAMP_EDGE:
		{
			glType = GL_MIRROR_CLAMP_TO_EDGE;
			break;
		}
		case WrapType::CLAMP_BORDER:
		{
			glType = GL_CLAMP_TO_BORDER;
			break;
		}
		case WrapType::REPEAT:
		{
			glType = GL_REPEAT;
			break;
		}
		case WrapType::MIRRORED_REPEAT:
		{
			glType = GL_MIRRORED_REPEAT;
			break;
		}
		default:
		{
			break;
		}
	}
	return glType;

}