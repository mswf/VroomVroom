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

void Importer::SetSceneImportFlags( int flags ) const
{
	imp_->SetSceneImportFlags( flags );
}

bool Importer::ImportObjFile( const std::string &pFile, bool importTextures )
{
	if ( Assets.MeshExists( pFile.c_str() ) )
	{
		Terminal.Warning("Mesh " + pFile + " already imported. Aborting redundant loading");
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
		Mesh* mesh;
		Material* material;
		std::vector< std::string > textures;
		
		// Mesh
		ImportMesh(scene, n, mesh, pFile.c_str() );
		
		// Material & Textures
		ImportMaterial( material, scene, mesh, textures );
		if ( importTextures ) ImportTextures( textures, material );
		
	}
	imp_->FreeScene(scene);
	return true;
}

void Importer::ImportMesh( aiScene *scene, const uint32& index, Mesh*& mesh, const char* name ) const
{
	const aiMesh* m = scene->mMeshes[index];
	imp_->ExtractMesh(m, mesh);
	mesh->materialId = m->mMaterialIndex;
	Assets.InsertMesh( name, mesh);
}

void Importer::ImportMaterial( Material*& material, aiScene* scene, Mesh* mesh, std::vector< std::string >& textures ) const
{
	aiMaterial *mtl = scene->mMaterials[ mesh->materialId ];
	imp_->ExtractMaterial( mtl, material, &textures );
	if ( Assets.MaterialExists( material->name.c_str() ) )
	{
		delete material;
		return;
	}
	mesh->materialId = ResourceManager::materialId++;
	Assets.InsertMaterial( mesh->materialId, material->name.c_str(), material );
	Terminal.Log("Material " + material->name + " has been created.", true);
}

void Importer::ImportTextures( const std::vector< std::string >& textures, Material* material )
{
	std::vector< std::string >::const_iterator iter = textures.begin();
	std::vector< std::string >::const_iterator end = textures.end();
	for ( ; iter != end; ++iter )
	{
		std::string name = (*iter);
		if ( ImportImage( name.c_str() ) )
		{
			SetTextureToMaterial( material, name );
		}
	}
}

void Importer::SetTextureToMaterial( Material* material, std::string name )
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

bool Importer::ImportImage( const char* filename, bool vertical_flip, FilterType minFilter, FilterType magFilter, WrapType wrapping ) const
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
		Terminal.Warning( "Failed to load image [" + string(filename) + "], error: " + imp_->import_image_failure_reason );
		std::string err_file( filename );
		Terminal.Warning( "Failed to load image [" + err_file + "], error: " + imp_->import_image_failure_reason );
		delete image;
		return false;
	}
	rm.InsertImage( filename, image );
	return true;
}

ImageData* Importer::ReImportImage( const char* filename, bool vertical_flip ) const
{
	std::string file( Content::GetPath() + "/" + filename );
	ImageData* image = new ImageData();
	image->components = 3;
	IMPORTER_MESSAGE imp_err = IMPORTER_MESSAGE::FILE_OK;
	image->pixelData = imp_->ImportImage( file.c_str(), image->width, image->height, image->components, imp_err, vertical_flip );
	if ( imp_err == IMPORTER_MESSAGE::IMAGE_FAILED_TO_LOAD )
	{
		std::string err_file( filename );
		Terminal.Warning( "Failed to load image [" + err_file + "], error: " + imp_->import_image_failure_reason );
		delete image;
		return nullptr;
	}
	return image;
}

