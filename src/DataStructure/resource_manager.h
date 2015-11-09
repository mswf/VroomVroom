#ifndef resource_manager_h
#define resource_manager_h

#include <string>
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
	void LoadMesh(const aiScene* s);
	void LoadTexture();
	const Mesh* CreateTriangleMesh();
	const Mesh* CreateCubeMesh( bool centered = true );
	
	Mesh* tempMesh;
	std::vector< Mesh* >* rMeshes;
	std::vector< int > rTextures;

};

#endif /* resource_manager_h */
