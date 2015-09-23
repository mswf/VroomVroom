#ifndef renderer_hpp
#define renderer_hpp

#include <glew.h>
#include <iostream>	// cout, cerr, endl
#include <fstream>	// fin,

void RenderObject( unsigned int );
void Draw();
void LoadModel();
void LoadTexture();
GLuint CreateTexture( unsigned char*, int, int );
void CreateCamera();
void CreateOrthogonalProjection();
void CreatePerspectiveProjection();
void CreateVAO();
void CreateFBO();
void GetAttribute();
void GetUniform();
void SetAttribute();
void SetUniform();
GLuint CreateShader( GLenum );
void CreateProgram();
void CompileShader( GLuint );
void LinkShader();
void ValidateShader();
void DeleteShaderProgram();
void GetShaderProgram();
const char * ReadFile( const char * );

#endif /* renderer_hpp */
