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
		ImageData* ReImportImage( const char* filename, bool vertical_flip = true  );
	protected:
		ImporterImp* imp_;
};

#endif /* importer_h */
