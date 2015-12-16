#include "importer.h"
#include "content.h"
#include "../console.h"
#include "../DataStructure/material.h"
#include "../DataStructure/resource_manager.h"

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

bool Importer::ImportObjFile( const std::string &pFile, bool importTextures )
{
	ResourceManager& rm = Assets;
	
	if ( rm.MeshExists( pFile.c_str() ) )
	{
		Terminal.Warning("Obj already imported. Aborting redundant loading");
		return false;
	}
	
	Terminal.Log("Importing obj: " + pFile);
	
	IMPORTER_MESSAGE imp_err;
	std::string path( Content::GetPath() + "/" + pFile );
	aiScene* scene = imp_->ImportObjFile( path, imp_err );
	
	if ( imp_err == IMPORTER_MESSAGE::FILE_NOT_FOUND )
	{
		Terminal.Log( string( "Couldn't open file: " + pFile ) );
		return false;
	}
	
	if ( imp_err == IMPORTER_MESSAGE::FILE_CORRUPT )
	{
		Terminal.Log( imp_->import_scene_failure_reason );
		return false;
	}
	
	unsigned int n;
	if (scene->mNumMeshes > 1)
	{
		Terminal.Warning("Object file " + pFile + " contains multiple meshes!");
	}
	
	for ( n = 0; n < scene->mNumMeshes; ++n )
	{
		// Mesh
		const aiMesh* m = scene->mMeshes[n];
		Mesh* mesh = new Mesh();
		imp_->ExtractMesh(m, mesh);
		mesh->materialId = 0;
		// Material
		// TODO(Valentinas): THIS CHECK IS A HACK AND SHOULD BE FIXED, ASSIMP ALWAYS HAS MATERIAL!
		if ( scene->HasMaterials() && scene->mMaterials[0]->mNumProperties > 9 )
		{
			aiMaterial *mtl = scene->mMaterials[ m->mMaterialIndex ];
			// TODO(Valentinas): THIS IS A MEMORY LEAK! FIX IT!
			Material* material = new Material();
			std::vector< std::string > textures;
			imp_->ExtractMaterial( mtl, material, &textures );
			material->shader->name = material->name.c_str();
			if (importTextures)
			{
				std::vector< std::string >::const_iterator iter = textures.begin();
				std::vector< std::string >::const_iterator end = textures.end();
				for ( ; iter != end; ++iter )
				{
					std::string name = (*iter);
					if ( ImportImage( name.c_str() ) )
					{
						// Set diffuse by to material default
						if ( name.find("_D") != std::string::npos )
						{
							//Terminal.Log("Found diffuse texture, setting to material.");
							material->SetDiffuseTexture( name.c_str() );
						}
						
						// Set normal by to material default
						if ( name.find("_N") != std::string::npos )
						{
							//Terminal.Log("Found normal texture, setting to material.");
							material->SetNormalTexture( name.c_str() );
						}
					}
				}
			}
			mesh->materialId = ResourceManager::materialId++;
			rm.InsertMaterial( mesh->materialId, material->name.c_str(), material );
		}
		if ( !rm.MeshExists( pFile.c_str() ) )
		{
			rm.InsertMesh( pFile.c_str(), mesh);
		}
		else
		{
			Terminal.Warning("Trying to insert mesh with an already taken name, skipping import!");
			//rm.MergeToExistingMesh( pFile.c_str(), mesh );
			delete mesh;
		}
	}
	imp_->FreeScene(scene);
	return true;
}

bool Importer::ImportImage( const char* filename, bool vertical_flip, FilterType minFilter, FilterType magFilter, WrapType wrapping )
{
	ResourceManager& rm = Assets;
	std::string file( Content::GetPath() + "/" + filename );
	// TODO(Valentinas): Memory leak here, fix me!!
	ImageData* image = new ImageData();
	image->magFilter = magFilter;
	image->minFilter = minFilter;
	image->wrap = wrapping;
	image->mipmapping = false;
	image->components = 3;
	IMPORTER_MESSAGE imp_err = IMPORTER_MESSAGE::FILE_OK;
	image->pixelData = imp_->ImportImage( file.c_str(), image->width, image->height, image->components, imp_err, vertical_flip );
	if ( imp_err == IMPORTER_MESSAGE::IMAGE_FAILED_TO_LOAD )
	{
		std::string err_file( filename );
		Terminal.Warning( "Failed to load image <" + err_file + ">, error: " + imp_->import_image_failure_reason );
		return false;
	}
	rm.InsertImage( filename, image );
	return true;
}

ImageData* Importer::ReImportImage( const char* filename, bool vertical_flip )
{
	std::string file( Content::GetPath() + "/" + filename );
	ImageData* image = new ImageData();
	image->components = 4;
	IMPORTER_MESSAGE imp_err = IMPORTER_MESSAGE::FILE_OK;
	image->pixelData = imp_->ImportImage( file.c_str(), image->width, image->height, image->components, imp_err, vertical_flip );
	if ( imp_err == IMPORTER_MESSAGE::IMAGE_FAILED_TO_LOAD )
	{
		std::string err_file( filename );
		Terminal.Warning( "Failed to load image <" + err_file + ">, error: " + imp_->import_image_failure_reason );
		return NULL;
	}
	return image;
}

