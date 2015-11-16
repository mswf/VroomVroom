#ifndef resource_manager_h
#define resource_manager_h

#include <string>
#include <map>
#include "glew.h"
#include "data_types.h"
#include "texture.h"
#include "scene.h"
#include "postprocess.h"

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	
	bool ImportObjFile( const std::string& pFile, int flags = aiProcessPreset_TargetRealtime_Quality );
	void LoadMesh( const aiScene* s );
	const unsigned int LoadTexture( const char* filename );
	const Mesh* CreateTriangleMesh();
	const Mesh* CreateCubeMesh( bool centered = true );
	const Mesh* CreateTetrahedronMesh();
	const Mesh* CreateIcosahedronMesh();
	const Mesh* CreateOctahedronMesh();
	
	Mesh* tempMesh;
	std::vector< Mesh* >* rMeshes;
	std::map< std::string, unsigned int > rTextures;

};

#endif /* resource_manager_h */

