#include "Texture.h"

GLuint BufferTexture2D( GLenum target, GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, GLvoid* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( target, textureId );
	
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( target, (*iter).first, (*iter).second );
	}
	glTexImage2D( target, levelOfDetail, internalFormat, width, height, 0, pixelFormat, type, data );
	if ( generateMipMap ) glGenerateMipmap( GL_TEXTURE_2D );
	
	return textureId;
}

GLuint BufferTexture1D( GLenum target, GLint levelOfDetail, GLint internalFormat, GLint width, GLint pixelFormat, GLenum type, GLvoid* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap )
{
	unsigned int textureId;
	glGenTextures( 1, &textureId );
	glBindTexture( target, textureId );
	
	std::vector< std::pair< GLenum, GLint > >::const_iterator iter = textureParameters->begin();
	std::vector< std::pair< GLenum, GLint > >::const_iterator end = textureParameters->end();
	for ( ; iter != end; ++iter )
	{
		glTexParameteri( target, (*iter).first, (*iter).second );
	}
	glTexImage1D( target, levelOfDetail, internalFormat, width, 0, pixelFormat, type, data );
	if ( generateMipMap ) glGenerateMipmap( GL_TEXTURE_1D );
	
	return textureId;
}

void DeleteTexture( GLuint textureId )
{
	glDeleteTextures(1, &textureId);
}