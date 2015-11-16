#include "importer.h"
#include "../console.h"

Importer::Importer()
{
	imp_ = new ImporterImp();
}

Importer::~Importer()
{
	delete imp_;
}

void Importer::SetSceneImportFlags( int flags )
{
	imp_->SetSceneImportFlags( flags );
}

bool Importer::ImportObjFile( const std::string &pFile )
{
	int imp_err = 1;// imp_->ImportObjFile(pFile);
	
	if ( (imp_err = (int)IMPORTER_MESSAGE::FILE_NOT_FOUND) )
	{
		Terminal.Log( string( "Couldn't open file: " + pFile ) );
		return false;
	}
	
	if ( (imp_err = (int)IMPORTER_MESSAGE::FILE_CORRUPT) )
	{
		Terminal.Log( imp_->import_scene_failure_reason );
		return false;
	}
	
	return true;
}

bool Importer::ImportImage( const char* filename )
{
	int imp_err = 1; //imp_->ImportImage(filename);
	if ( (imp_err = (int)IMPORTER_MESSAGE::TEXTURE_FAILED_TO_LOAD) )
	{
		Terminal.Log( "Failed to load texture, error: " + imp_->import_image_failure_reason );
		return false;
	}
	return true;
}
