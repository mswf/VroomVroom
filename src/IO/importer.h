#ifndef importer_h
#define importer_h

#include "importer_imp.h"

class Importer
{
	public:
	
		Importer();
		~Importer();
		void SetSceneImportFlags( int flags );
		bool ImportObjFile( const std::string& pFile );
		bool ImportImage( const char* filename );
	
	protected:
		ImporterImp* imp_;
};

#endif /* importer_h */
