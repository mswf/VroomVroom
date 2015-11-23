#ifndef Texture_h
#define Texture_h

#include "glew.h"
#include <map>
#include <vector>

// TODO(Valentinas): Add possibility to allocate or deallocate multiple textures in one go
// TODO(Valentinas): Support odd texture dimensions

GLuint BufferTexture2D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, unsigned char* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap = false );

GLuint BufferTexture1D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint pixelFormat, GLenum type, GLvoid* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap = false );

void BindTexture( GLenum textureUnit, GLenum target, GLuint program );
void UnbindTexture();

void DeleteTexture( GLuint textureId );

#endif /* Texture_h */

/*

	// we also want to be able to deal with odd texture dimensions
 	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
	glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
	glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
 
*/