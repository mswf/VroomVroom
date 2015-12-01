#ifndef importer_imp_h
#define importer_imp_h

#include <string>
#include "scene.h"

//TODO(Valentinas): Don't use data_type, if portability is prefered
#include "../DataStructure/data_types.h"

class Material;

enum class IMPORTER_MESSAGE
{
	FILE_OK = 0,
	FILE_NOT_FOUND = 1,
	FILE_CORRUPT = 2,
	IMAGE_LOAD_OK = 3,
	IMAGE_FAILED_TO_LOAD = 4
};

class ImporterImp
{
	public:

		ImporterImp();
		~ImporterImp();

		void FreeScene( aiScene* sc );
		void FreeImage( unsigned char* img );
		void ExtractMesh( const aiMesh* mesh, Mesh* m );
		void ExtractMaterial( const aiMaterial* mtl, Material* material, std::vector< std::string >* textureIdMap );
		aiScene* ImportObjFile( const std::string& pFile, IMPORTER_MESSAGE& message );
		unsigned char* ImportImage( const char* filename, unsigned int& width, unsigned int& height, unsigned int requiring_components, IMPORTER_MESSAGE& message, bool vertical_flip = true );
		void SetSceneImportFlags( int flags )
		{
			importFlags = flags;
		}

		std::string import_scene_failure_reason;
		std::string import_image_failure_reason;

	protected:

		int importFlags;
};


#endif /* importer_imp_h */

/*
 struct MaterialProperties
 {
	std::string name;
	int wireframe_enabled;
	int two_sided;
	unsigned int textureCount;
	float shininess;
	float shininess_strenght;
	float opacity;
	float bump_scaling;
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float transparent[4];
	float reflective[4];
	std::vector< std::string > textureIdMap;
	MaterialProperties() :
 name( "No name" ),
 wireframe_enabled(0),
 two_sided(0),
 textureCount(0),
 shininess(0.0f),
 shininess_strenght(0.0f),
 opacity(0.0f),
 bump_scaling(0.0f),
 diffuse { 0.2f, 0.2f, 0.2f, 1.0f },
 ambient { 0.0f, 0.0f, 0.0f, 1.0f },
 specular { 0.0f, 0.0f, 0.0f, 1.0f },
 emissive { 0.0f, 0.0f, 0.0f, 1.0f },
 transparent { 0.0f, 0.0f, 0.0f, 1.0f },
 reflective { 0.0f, 0.0f, 0.0f, 1.0f },
 textureIdMap { 0 }
	{}
 };
 */