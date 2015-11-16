#ifndef importer_imp_h
#define importer_imp_h

#include <string>
#include "scene.h"
#include <vector>

enum class IMPORTER_MESSAGE
{
	FILE_OK = 0,
	FILE_NOT_FOUND = 1,
	FILE_CORRUPT = 2,
	TEXTURE_LOAD_OK = 3,
	TEXTURE_FAILED_TO_LOAD = 4
};

struct MaterialProperties
{
	std::string name;
	int wireframe_enabled;
	int two_sided;
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
};

class ImporterImp
{
	public:
	
		ImporterImp();
		~ImporterImp();
	
		virtual void FreeScene( aiScene* sc );
		virtual void FreeImage( unsigned char* img );
		virtual void ExtractMesh( const aiMesh* mesh );
		virtual void ExtractMaterial( const aiMaterial* material, MaterialProperties* properties );
		virtual int ImportObjFile( const std::string& pFile, aiScene* scene );
		virtual int ImportImage( const char* filename, unsigned char* image, int& width, int& height );
		virtual void SetSceneImportFlags( int flags ) { importFlags = flags; }
	
		std::string import_scene_failure_reason;
		std::string import_image_failure_reason;
	
	protected:
	
		int importFlags;
};


#endif /* importer_imp_h */
