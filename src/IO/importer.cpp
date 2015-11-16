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
	int imp_err = imp_->ImportObjFile(pFile);
	
	if ( (imp_err = (int)IMPORTER_MESSAGE::FILE_NOT_FOUND) )
	{
		Terminal.Log( string( "Couldn't open file: " + pFile ) );
		return false;
	}
	
	if ( (imp_err = (int)IMPORTER_MESSAGE::FILE_CORRUPT) )
	{
		Terminal.Log( imp_->GetError( IMPORTER_ERROR_TYPE::SCENE ) );
		return false;
	}
	
	return true;
}

bool Importer::ImportImage( const char* filename )
{
	int imp_err = imp_->ImportImage(filename);
	if ( (imp_err = (int)IMPORTER_MESSAGE::TEXTURE_FAILED_TO_LOAD) )
	{
		Terminal.Log( "Failed to load texture, error: " + imp_->GetError( IMPORTER_ERROR_TYPE::IMAGE ) );
		return false;
	}
	return true;
}
