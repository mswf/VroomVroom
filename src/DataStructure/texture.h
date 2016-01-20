#ifndef Texture_h
#define Texture_h

#include "glew.h"
#include <vector>

// TODO(Valentinas): Add parameter for changing Texture Wrapping mode
// TODO(Valentinas): Add possibility to allocate or deallocate multiple textures in one go
// TODO(Valentinas): Support odd texture dimensions
// TODO(Valentinas): Set the number of generated mip map levels
/*
	// we also want to be able to deal with odd texture dimensions
	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glPixelStorei( GL_UNPACK_ROW_LENGTH, 0 );
	glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
	glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
 
 */

enum class FilterType;
enum class WrapType;
enum class CubeMap;

GLuint BufferTexture3D( GLint levelOfDetail, GLint internalFormat, GLint width, GLint height, GLint depth, GLint pixelFormat, GLenum type, unsigned char* data, const std::vector< std::pair< GLenum, GLint > >* textureParameters, bool generateMipMap = false );

GLuint BufferTexture2D( GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum dataType, unsigned char* data, FilterType magFilter, FilterType minFilter, WrapType wrap, bool generateMipMap = false );

GLuint BufferTexture1D( GLint internalFormat, GLint width, GLint pixelFormat, GLenum type, GLvoid* data, bool filterNearest, bool generateMipMap = false );

GLuint BufferCubeMap( GLint size, bool mipmap, unsigned char* nx, unsigned char* ny, unsigned char* nz, unsigned char* px, unsigned char* py, unsigned char* pz );

void BufferTextureCubeMap( GLuint mapId, GLenum sideTarget, GLint internalFormat, GLint width, GLint height, GLint pixelFormat, GLenum type, bool generateMipMap, unsigned char* data );

void UpdateBufferImage1D( GLuint texture, GLint offX, GLint offY, GLint w, GLint h, GLenum pixelFormat, GLenum dataType, unsigned char* data );

void UpdateBufferImage2D( GLuint texture, GLint offX, GLint offY, GLint w, GLint h, GLenum pixelFormat, GLenum dataType, unsigned char* data, bool mipmap = false );

void SetTextureWrapping( GLuint texture, GLenum target, GLenum wrap, WrapType type );

void SetTextureFilter( GLuint texture, GLenum target, FilterType minFilter, FilterType magFilter );

GLenum GetFilterParameter( FilterType type );
GLenum GetWrapParameter( WrapType type );
GLenum GetCubeMapFace( CubeMap type );

void BindTexture( GLenum textureUnit, GLenum target, GLuint program );
void UnbindTexture( GLenum target );

void DeleteTexture( GLuint textureId );

#endif /* Texture_h */

