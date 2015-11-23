#include "Texture.h"

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

void BindTexture( GLenum textureUnit, GLenum target, GLuint program )
{
	glActiveTexture( textureUnit );
	glBindTexture( target, program );
}

void UnbindTexture()
{
	glBindTexture(GL_ACTIVE_TEXTURE, 0);
}

void DeleteTexture( GLuint textureId )
{
	glDeleteTextures(1, &textureId);
}