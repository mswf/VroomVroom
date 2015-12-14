#ifndef importer_imp_h
#define importer_imp_h

#include "../DataStructure/data_types.h"
#include <string>
#include "scene.h"

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
		void ExtractMesh( const aiMesh* mesh, Mesh* m );
		void ExtractMaterial( const aiMaterial* mtl, Material* material, std::vector< string >* textureIdMap );
		aiScene* ImportObjFile( const string& pFile, IMPORTER_MESSAGE& message );
		unsigned char* ImportImage( const char* filename, uint32& width, uint32& height, uint32 requiring_components, IMPORTER_MESSAGE& message, bool vertical_flip = true );
		inline void SetSceneImportFlags( const int32& flags ) { importFlags = flags; }

		string import_scene_failure_reason;
		string import_image_failure_reason;

	protected:

		int importFlags;
};


#endif /* importer_imp_h */