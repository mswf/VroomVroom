#ifndef mesh_h
#define mesh_h

#include "data_types.h"

void BufferMesh( const Mesh* m, ModelInstance* instance );
void BufferUpdate( unsigned buffer, unsigned offset, unsigned length, void* data );
void UnBuffer( ModelInstance* instance );
void BufferPoints( unsigned int& vao, unsigned int& vbo, const std::vector< glm::vec3 >& points, const std::vector< glm::vec3 >& colours );

#endif /* mesh_h */
