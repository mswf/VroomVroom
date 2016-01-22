#ifndef mesh_h
#define mesh_h

#include "data_types.h"

void BufferMesh( const Mesh* m, ModelInstance* instance );
void BufferUpdate( unsigned buffer, unsigned offset, unsigned length, void* data );

void CreateDynamicBuffer( unsigned int& vao, unsigned int& vbo, unsigned long bufferSize );
void BufferPoints( unsigned int& vao, unsigned int& vbo, const std::vector< glm::vec3 >& points, const std::vector< glm::vec4 >& colours );
void DeleteArrayBuffer( unsigned int& vbo );
void DeleteVertexArray( unsigned int& vao );
void UnBuffer( ModelInstance* instance );

#endif /* mesh_h */
