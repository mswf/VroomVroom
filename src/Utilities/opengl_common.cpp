#include "opengl_common.h"
#include "../console.h"

void CheckGlError( const char* caller )
{
	GLenum error_found = glGetError();
	if ( error_found != GL_NO_ERROR )
	{
		std::string err;
		switch ( error_found )
		{
			case GL_INVALID_ENUM:
			{
				err = " Invalid Enum.";
				break;
			}
			case GL_INVALID_VALUE:
			{
				err = " Invalid value.";
				break;
			}
			case GL_INVALID_OPERATION:
			{
				err = " Invalid operation.";
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				err = " Invalid framebuffer operation.";
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				err = " Stack overflow.";
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				err = " Stack underflow.";
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				err = " Out of memory.";
				break;
			}
			default:
				err = " UNKNOWN.";
				break;
		}
		err = std::string("Error from caller " + std::string(caller) + "," + err);
		Terminal.LogOpenGL( err, true );
	}
}
