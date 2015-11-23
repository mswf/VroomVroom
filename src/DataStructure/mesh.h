#ifndef mesh_h
#define mesh_h

#include "data_types.h"

class Material;

void BufferMesh( const Mesh* m, ModelInstance* instance, Material* mtl );
void UnBuffer( ModelInstance* instance );

#endif /* mesh_h */
