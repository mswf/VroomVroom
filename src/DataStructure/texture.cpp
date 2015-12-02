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
	if ( generateMipMap ) glGenerateMipmap( GL_TEXTURE_3D );
	
	return textureId;
}


GLuint BufferTexture2D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, unsigned char* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_2D, textureId );
	
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( GL_TEXTURE_2D, (*iter).first, (*iter).second );
	}
	glTexImage2D( GL_TEXTURE_2D, levelOfDetail, internalFormat, width, height, 0, pixelFormat, type, (GLvoid*)data );
	if ( generateMipMap ) glGenerateMipmap( GL_TEXTURE_2D );
	
	return textureId;
}

GLuint BufferTexture1D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint pixelFormat, GLenum type, GLvoid* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( GL_TEXTURE_1D, textureId );
	
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( GL_TEXTURE_1D, (*iter).first, (*iter).second );
	}
	glTexImage1D( GL_TEXTURE_1D, levelOfDetail, internalFormat, width, 0, pixelFormat, type, data );
	if ( generateMipMap ) glGenerateMipmap( GL_TEXTURE_1D );
	
	return textureId;
}

void BufferTextureCubeMap( GLuint mapId, GLenum sideTarget, GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, unsigned char* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters )
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, mapId );
	
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( GL_TEXTURE_CUBE_MAP, (*iter).first, (*iter).second );
	}
	glTexImage2D( sideTarget, levelOfDetail, internalFormat, width, height, 0, pixelFormat, type, (GLvoid*)data );
}

void BindTexture( GLenum textureUnit, GLenum target, GLuint program )
{
	glActiveTexture( textureUnit );
	glBindTexture( target, program );
}

// TODO(Valentinas): I should check whether the glBindTexture to zero after drawing is necessary
void UnbindTexture( GLenum target )
{
	glBindTexture( target, 0 );
}

void DeleteTexture( GLuint textureId )
{
	glDeleteTextures( 1, &textureId );
}