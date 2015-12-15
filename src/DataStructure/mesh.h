#ifndef mesh_h
#define mesh_h

#include "data_types.h"

void BufferMesh( const Mesh* m, ModelInstance* instance );
void BufferUpdate( unsigned buffer, unsigned offset, unsigned length, void* data );
void UnBuffer( ModelInstance* instance );
void CreateDynamicBuffer( unsigned int& vao, unsigned int& vbo, unsigned long bufferSize );
void BufferPoints( unsigned int& vao, unsigned int& vbo, const std::vector< glm::vec3 >& points, const std::vector< glm::vec4 >& colours );
void BufferClear( unsigned int vbo );

#endif /* mesh_h */
