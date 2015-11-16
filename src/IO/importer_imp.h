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

enum class IMPORTER_ERROR_TYPE
{
	SCENE,
	IMAGE
};

class ImporterImp
{
	public:
	
		ImporterImp();
		~ImporterImp();
	
		virtual void FreeScene( aiScene* sc );
		virtual void FreeImage( unsigned char* img );
		virtual void ExtractMesh( const aiMesh* mesh );
		virtual void ExtractMaterial( const aiMaterial* material );
		//virtual void ExtractTextures();
		virtual int ImportObjFile( const std::string& pFile );
		virtual int ImportImage( const char* filename );
		virtual const std::string& GetError( IMPORTER_ERROR_TYPE type ) const;
		virtual void SetSceneImportFlags( int flags );
	
	protected:
	
		aiScene* loadedScene;
		unsigned char* loadedImage;
	
		int importFlags;
		std::string importSceneError;
		std::string importImageError;
	
	private:
	
		std::string unknownError = "Unknown error type";
};


#endif /* importer_imp_h */
