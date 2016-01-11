#ifndef importer_h
#define importer_h

#include "importer_imp.h"

class Importer
{
	public:
	
		Importer();
		~Importer();
		void SetSceneImportFlags( int flags );
		bool ImportObjFile( const std::string& pFile, bool importTextures = true );
		bool ImportImage( const char* filename, bool vertical_flip = true, FilterType minFilter = FilterType::LINEAR, FilterType magFilter = FilterType::LINEAR, WrapType wrapping = WrapType::REPEAT );
		void ImportMesh( aiScene* scene, const uint32& index, Mesh*& mesh, const char* name );
		void ImportMaterial( Material*& material, aiScene* scene, Mesh* mesh, std::vector< std::string >& textures );
		void ImportTextures( const std::vector< std::string >& textures, Material* material );
		void SetTextureToMaterial( Material* material, std::string name );
		ImageData* ReImportImage( const char* filename, bool vertical_flip = true  );
	protected:
		ImporterImp* imp_;
};

#endif /* importer_h */
