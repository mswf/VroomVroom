#ifndef mesh_h
#define mesh_h

#include "data_types.h"

void BufferMesh( const Mesh* m, ModelInstance* instance );
void UnBuffer( ModelInstance* instance );
unsigned int BufferPoints( const std::vector< glm::vec3 >& points, const std::vector< glm::vec3 >& colours );

#endif /* mesh_h */
