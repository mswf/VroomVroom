#include "resource_manager.h"
#include "../console.h"
#include "texture.h"
#include "mesh.h"

//TODO(Valentinas): Reload meshes
ResourceManager::~ResourceManager()
{
	images.clear();
	meshes.clear();
	materials.clear();
}

void ResourceManager::UpdateMeshBuffer()
{
	std::map< std::string, Mesh* >::const_iterator iter = meshes.begin();
	std::map< std::string, Mesh* >::const_iterator end = meshes.end();
	for ( ; iter != end; ++iter )
	{
		if ( (*iter).second->hasBufferChanged )
		{
			printf( "Changed %s", (*iter).first.c_str() );
		}
	}
}

ModelInstance* ResourceManager::GetModel( const char* name )
{
	std::map< std::string, ModelInstance* >::const_iterator iter_model = models.find(name);
	std::map< std::string, Mesh* >::const_iterator iter_mesh = meshes.find(name);

	std::string temp_name(name);
	if ( iter_model == models.end() || !(*iter_mesh).second->isBuffered )
	{
		//std::string warning_msg( temp_name + " has not been buffered yet." );
		//Terminal.Log( warning_msg );
		
		// Checking if the mesh is imported
		if ( iter_mesh == meshes.end() )
		{
			std::string err_msg( "No mesh by the name of" + temp_name + " has been found in resources." );
			Terminal.Warning( err_msg );
			return NULL;
		}
		
		//Buffer ModelInstance & add to resource list
		ModelInstance* newInstance = new ModelInstance();
		unsigned int mtl = (*iter_mesh).second->materialId;
		BufferMesh( (*iter_mesh).second, newInstance, GetMaterialById( mtl ) );
		
		// Set the mesh's buffer to true
		(*iter_mesh).second->isBuffered = true;
		
		InsertModelInstance( name, newInstance );
		
		return GetModel( name );
	}
	return (*iter_model).second;
}

Material* ResourceManager::GetMaterialById( unsigned int materialId )
{
	return materials.at( materialId );
}

Material* ResourceManager::GetMaterialByName( const char* name )
{
	
	return materials.at( GetMaterialId(name) );
}

unsigned int ResourceManager::GetMaterialId( const char* name )
{
	std::map< std::string, unsigned int >::const_iterator iter_mtl = materialIds.find(name);
	if ( iter_mtl == materialIds.end() )
	{
		Terminal.Warning( std::string("Material: " + std::string(name) + " not found." ) );
		return 0;
	}
	return materialIds.at( name );
}

unsigned int ResourceManager::GetImageId( const char *name )
{
	std::map< std::string, unsigned int >::const_iterator iter_image = imageIds.find(name);
	if ( iter_image == imageIds.end() )
	{
		//Terminal.Warning( std::string("Image: " + std::string(name) + " not buffered." ) );
		if ( BufferImage2D(name) )
		{
			return GetImageId(name);
		}
		else
		{
			return 0;
		}
	}
	return imageIds.at(name);
}

ImageData* ResourceManager::GetImageData( const char* name )
{
	std::map< std::string, ImageData* >::const_iterator iter_image = images.find(name);
	if ( iter_image == images.end() )
	{
		Terminal.Warning( "Image not imported." );
		return NULL;
	}
	return images.at(name);
}

void ResourceManager::BufferImage1D( const char* name )
{
	
}

bool ResourceManager::BufferImage2D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL) return false;
	std::map< std::string, unsigned int >::const_iterator iter_imageId = imageIds.find(name);
	
	if ( !img->isBuffered || iter_imageId == imageIds.end() )
	{
		std::vector< std::pair<GLenum, GLint> >* textureParameters = new std::vector< std::pair<GLenum, GLint> >();
		std::pair< GLenum, GLint > param( GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		std::pair< GLenum, GLint > param2( GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		textureParameters->push_back( param );
		textureParameters->push_back( param2 );
		
		//HelperFunctions::PrintImageData(img->pixelData, img->width, img->height);
		
		img->imageId = BufferTexture2D( 0, GL_RGBA, img->width, img->height, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, textureParameters );
		img->isBuffered = true;
		imageIds.insert( std::pair< std::string, unsigned int >( std::string(name), img->imageId ) );
	}
	return true;
}

void ResourceManager::InsertModelInstance( const char* name, ModelInstance* instance )
{
	models.insert( std::pair< std::string, ModelInstance* >( std::string(name), instance ) );
}

void ResourceManager::InsertMesh( const char* name, Mesh* data )
{
	meshes.insert( std::pair< std::string, Mesh* >( std::string(name), data ) );
}

void ResourceManager::InsertMaterial( unsigned int id, const char* name, Material* data )
{
	
	materialIds.insert( std::pair< std::string, unsigned int >( std::string(name), id ) );
	materials.insert( std::pair< unsigned int, Material* >( id, data ) );
}

void ResourceManager::InsertImage( const char* name, ImageData* data )
{
	images.insert( std::pair< std::string, ImageData* >( std::string(name), data ) );
}

bool ResourceManager::MeshExists( const char *name )
{
	std::map< std::string, Mesh* >::const_iterator iter_mesh = meshes.find(name);
	if ( iter_mesh != meshes.end() )
	{
		return true;
	}
	return false;
}

bool ResourceManager::ImageExists( const char *name )
{
	std::map< std::string, ImageData* >::const_iterator iter_image = images.find(name);
	if ( iter_image != images.end() )
	{
		return true;
	}
	return false;
}