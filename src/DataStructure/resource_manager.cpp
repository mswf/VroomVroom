#include "resource_manager.h"
#include "data_types.h"
#include "texture.h"
#include "mesh.h"
#include "material.h"
#include "shader.h"
#include "../content.h"
#include "../console.h"
#include "../Utilities/helperFunctions.h"

uint32 ResourceManager::materialId = 0;
const char * DEFAULT_VERTEX = "shaders/default_vert.glsl";
const char * DEFAULT_FRAGMENT = "shaders/default_frag.glsl";

// TODO(Valentinas): SetBounds & find center when importing mesh

ResourceManager::ResourceManager()
{}
ResourceManager::~ResourceManager()
{
	images.clear();
	meshes.clear();
	materials.clear();
	shaderObjects.clear();
	shaderPrograms.clear();
}

void ResourceManager::Initialize()
{
	bool vertexDefault = ImportShader( DEFAULT_VERTEX, GLSLShaderType::VERTEX );
	bool fragmentDefault = ImportShader( DEFAULT_FRAGMENT, GLSLShaderType::FRAGMENT );
	if ( !(vertexDefault && fragmentDefault) )
	{
		Terminal.Warning( "Default shaders were missing. Loading builtin shaders." );
		LoadBuiltinShader();
	}
	else
	{
		const char* shaders[2] = { DEFAULT_VERTEX, DEFAULT_FRAGMENT };
		CreateShaderProgram( "Default", shaders, 2);
	}

	Material* defaultMat = new Material();
	defaultMat->SetShader( GetShaderProgram("Default") );
	defaultMat->name = "Default";
	InsertMaterial(0, "Default", defaultMat );
	++materialId;
}

void ResourceManager::LoadBuiltinShader()
{
	CreateShaderObject( DEFAULT_VERTEX, builtin_vertex, GLSLShaderType::VERTEX );
	CreateShaderObject( DEFAULT_FRAGMENT, builtin_fragment, GLSLShaderType::FRAGMENT );
	const char* shaders[2] = { DEFAULT_VERTEX, DEFAULT_FRAGMENT };
	CreateShaderProgram( "Default", shaders, 2);
}

// Meshes

Mesh* ResourceManager::GetMesh( const char* name ) const
{
	if ( !MeshExists( name ) )
	{
		Terminal.Warning( "Mesh: " + string(name) + " not imported." );
		return NULL;
	}
	return meshes.at(name);
}

bool ResourceManager::ImportMesh( const char* name )
{
	return importer.ImportObjFile( name );
}

bool ResourceManager::ImportMesh( const StringVector& files, StringVector& err_f )
{
	bool final = true;
	StringVector::const_iterator it = files.begin();
	StringVector::const_iterator end = files.end();
	for (; it != end; ++it )
	{
		if ( !importer.ImportObjFile( *it ) )
		{
			err_f.push_back( *it );
			final = false;
		}
	}
	return final;
}

void ResourceManager::UpdateMeshBuffer()
{
	Meshes::const_iterator it = meshes.begin();
	Meshes::const_iterator end = meshes.end();
	for ( ; it != end; ++it )
	{
		if ( it->second->hasBufferChanged )
		{
			printf( "Changed %s\n", it->first.c_str() );
		}
	}
}

// DON'T USE THIS FUNCTION< NOT WORKING PROPERLY
void ResourceManager::MergeToExistingMesh( const char* name, Mesh* data )
{
	Mesh* mesh = GetMesh(name);
	if ( mesh->hasPositions && data->hasPositions )
	{
		mesh->vertices.insert(  std::end( mesh->vertices ), std::begin( data->vertices ), std::end( data->vertices ) );
		mesh->indices.insert(  std::end( mesh->indices ), std::begin( data->indices ), std::end( data->indices ) );
		mesh->numIndices += data->numIndices;
	}
	if ( mesh->hasNormals && data->hasNormals )
	{
		mesh->normals.insert(  std::end( mesh->normals ), std::begin( data->normals ), std::end( data->normals ) );
	}
	if ( mesh->hasUVs && data->hasUVs )
	{
		mesh->uvs.insert(  std::end( mesh->uvs ), std::begin( data->uvs ), std::end( data->uvs ) );
	}
	if ( mesh->hasTangentsAndBitangets && data->hasTangentsAndBitangets )
	{
		mesh->tangents.insert(  std::end( mesh->tangents ), std::begin( data->tangents ), std::end( data->tangents ) );
		mesh->bitangents.insert(  std::end( mesh->bitangents ), std::begin( data->bitangents ), std::end( data->bitangents ) );
	}
}

void ResourceManager::SetMeshScale( const char* name, float scale )
{
	Mesh* mesh = GetMesh(name);
	if ( mesh->hasPositions )
	{
		mesh->scaleFactor = scale;
		std::vector< glm::vec3 >::iterator it = mesh->vertices.begin();
		std::vector< glm::vec3 >::const_iterator end = mesh->vertices.end();
		for ( ; it != end; ++it)
		{
			(*it) *= scale;
		}
	}
}

void ResourceManager::InsertMesh( const char* name, Mesh* data )
{
	meshes.insert( std::make_pair( string(name), data ) );
}

bool ResourceManager::MeshExists( const char* name ) const
{
	Meshes::const_iterator it_mesh = meshes.find(name);
	if ( it_mesh == meshes.end() )
	{
		// Mesh does not exists
		return false;
	}

	// Mesh exists
	return true;
}

// Model Instances

ModelInstance* ResourceManager::GetModel( const char* name )
{
	Models::const_iterator it_model = models.find(name);
	Meshes::const_iterator it_mesh = meshes.find(name);

	if ( it_model == models.end() )
	{
		if ( it_mesh != meshes.end() && !(*it_mesh).second->isBuffered)
		{
			//Terminal.Log( string(name) + " has not been buffered yet." );

			//Buffer ModelInstance & add to resource list
			ModelInstance* newInstance = new ModelInstance();
			unsigned int mtl = (*it_mesh).second->materialId;
			newInstance->materialId = mtl;
			BufferMesh( (*it_mesh).second, newInstance );

			// Set the mesh's buffer to true
			(*it_mesh).second->isBuffered = true;

			InsertModelInstance( name, newInstance );

			return GetModel( name );
		}

		// Checking if the mesh is imported
		if ( it_mesh == meshes.end() )
		{
			Terminal.Warning( "No mesh by the name of" + string(name) + " has been found in resources." );
			return NULL;
		}
	}
	return (*it_model).second;
}

void ResourceManager::InsertModelInstance( const char* name, ModelInstance* instance )
{
	models.insert( std::make_pair( string(name), instance ) );
}

// Images

bool ResourceManager::ImportImage( const char* name, bool vertical_flip )
{
	if ( ImageExists(name) )
	{
		Terminal.Warning( "Image already imported. Aborting redundant loading" );
		return false;
	}
	return importer.ImportImage( name, vertical_flip );
}

bool ResourceManager::ReImportImage( const char* name, bool vertical_flip )
{
	if ( ImageExists(name) )
	{
		ImageData* data = importer.ReImportImage( name, vertical_flip );
		ImageData* old = GetImageData(name);
		// ???? !!!!! ????
		old->pixelData = data->pixelData;
		UpdateImage2DBuffer(name);
		return true;
	}
	Terminal.Warning( "Image not imported at all. Using incorrect function?" );
	return false;
}

bool ResourceManager::ImportImage( const StringVector& files, StringVector& failedFile, bool vertical_flip )
{
	bool success = true;
	StringVector::const_iterator item = files.begin();
	StringVector::const_iterator end = files.end();
	for ( ; item != end; ++item )
	{
		if ( !ImportImage( item->c_str(), vertical_flip ) )
		{
			failedFile.push_back( *item );
			success = false;
		}
	}
	return success;
}

void ResourceManager::InsertImage( const char* name, ImageData* data )
{
	images.insert( std::make_pair( string(name), data ) );
}

uint32 ResourceManager::GetImageId( const char *name )
{
	StringToID::const_iterator it_image = imageIds.find(name);
	if ( it_image == imageIds.end() )
	{
		//Terminal.Warning( "Image: " + string(name) + " not buffered." );
		if ( BufferImage2D(name) )
		{
			return GetImageId(name);
		}
		else
		{
			Terminal.Warning( "Image not buffered, returning 0." );
			return 0;
		}
	}
	return imageIds.at(name);
}

ImageData* ResourceManager::GetImageData( const char* name ) const
{
	if ( !ImageExists( name ) )
	{
		Terminal.Warning( "Image: " + string(name) + " not imported." );
		return NULL;
	}
	return images.at(name);
}

bool ResourceManager::BufferImage1D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL)
	{
		return false;
	}
	StringToID::const_iterator it_imageId = imageIds.find(name);

	if ( !img->isBuffered || it_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture1D( GL_RGB, img->width, GL_RGB, GL_UNSIGNED_BYTE, img->pixelData, false );
		img->isBuffered = true;
		img->mipmapping = false;
		imageIds.insert( std::make_pair( string(name), img->imageId ) );
	}
	return true;
}

bool ResourceManager::BufferImage2D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL)
	{
		return false;
	}
	StringToID::const_iterator it_imageId = imageIds.find(name);

	if ( !img->isBuffered || it_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture2D( GL_RGB, img->width, img->height, GL_RGB, GL_UNSIGNED_BYTE,
										img->pixelData, img->magFilter, img->minFilter, img->wrap, img->mipmapping);
		img->isBuffered = true;
		imageIds.insert( std::make_pair( string(name), img->imageId ) );
		delete[] img->pixelData;
	}
	return true;
}

bool ResourceManager::BufferImage3D( const char* name )
{
	Terminal.Log( "Attempting to use BufferImage3D. No implementation present." );
	return false;
}

bool ResourceManager::UpdateImage2DBuffer( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL)
	{
		Terminal.Warning("Requested texture not found.");
		return false;
	}
	StringToID::const_iterator it_imageId = imageIds.find(name);
	if ( img->isBuffered || it_imageId != imageIds.end() )
	{
		UpdateBufferImage2D( img->imageId, 0, 0, img->width, img->width, GL_RGB, GL_UNSIGNED_BYTE, img->pixelData, img->mipmapping );
		return true;
	}
	return false;
}

unsigned int ResourceManager::CreateCubeMap( const std::vector< std::pair< uint8*, uint32 > >* textures, int32 width, int32 height )
{
	uint32 cubeMapId;
	glGenTextures( 1, &cubeMapId );

	std::vector< std::pair< uint8*, uint32 > >::const_iterator it = textures->begin();
	std::vector< std::pair< uint8*, uint32 > >::const_iterator end = textures->end();
	
	for ( ; it != end; ++it )
	{
		BufferTextureCubeMap(cubeMapId, it->second, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, it->first);
		delete[] it->first;

	}
	return cubeMapId;
}


bool ResourceManager::ImageExists( const char* name ) const
{
	Images::const_iterator it_image = images.find(name);
	if ( it_image != images.end() )
	{
		// Image exists
		return true;
	}
	// Image does not exists
	return false;
}

// Materials

Material* ResourceManager::GetMaterialById( uint32 materialId ) const
{
	if ( !MaterialExists(materialId) )
	{
		Terminal.Warning( "Material id <" + std::to_string(materialId) + "> not found." );
		return NULL;
	}
	return materials.at( materialId );
}

Material* ResourceManager::GetMaterialByName( const char* name ) const
{
	return materials.at( GetMaterialId(name) );
}

uint32 ResourceManager::GetMaterialId( const char* name ) const
{
	if ( !MaterialExists(name) )
	{
		Terminal.Warning( "Material: " + string(name) + " not found." );
		return 0;
	}
	return materialIds.at( name );
}

void ResourceManager::InsertMaterial( uint32 id, const char* name, Material* data )
{
	if ( !MaterialExists(name) )
	{
		materialIds.insert( std::make_pair( string(name), id ) );
		materials.insert( std::make_pair( id, data ) );
	}
}

bool ResourceManager::MaterialExists( const char* name ) const
{
	StringToID::const_iterator it_mtl = materialIds.find(name);
	if ( it_mtl != materialIds.end() )
	{
		// Material exists
		return true;
	}
	// Material does not exists
	return false;
}

bool ResourceManager::MaterialExists( const uint32 id ) const
{
	Materials::const_iterator it_mtl = materials.find(id);
	if ( it_mtl != materials.end() )
	{
		// Material exists
		return true;
	}
	// Material does not exists
	return false;
}


// Shaders

bool ResourceManager::ImportShader( const char* name, GLSLShaderType type )
{
	// Check if exists in resources
	if ( ShaderObjectExists( name ) )
	{
		return false;
	}
	// Check if file exists in directory
	string path( Content::GetPath() + "/" + name );
	if ( HelperFunctions::FileExists( path.c_str() ) )
	{
		// Get source
		string source = HelperFunctions::ReadFile( path );

		// Create shader object
		CreateShaderObject(name, source.c_str(), type);
		return true;
	}
	return false;
}

bool ResourceManager::ReImportShader( const char* name, GLSLShaderType type = GLSLShaderType::UNKNOWN )
{
	// Check if exists in resources
	if ( !ShaderObjectExists( name ) )
	{
		return false;
	}
	// Check if file exists in directory
	std::string path(Content::GetPath() + "/" + name);
	if ( HelperFunctions::FileExists( path.c_str() ) )
	{
		// Get source
		Terminal.Log( string("Reimporting shader: ") + name, true );
		string source = HelperFunctions::ReadFile( path );
		UpdateShaderProgram( name, type, source.c_str() );
		return true;
	}
	return false;
}


bool ResourceManager::ImportShader( const std::vector< std::pair< string, GLSLShaderType > >& list, StringVector& err_f )
{
	bool final = true;
	std::vector< std::pair< string, GLSLShaderType > >::const_iterator it = list.begin();
	std::vector< std::pair< string, GLSLShaderType > >::const_iterator end = list.end();
	for ( ; it != end; ++it )
	{
		if ( !ImportShader( it->first.c_str(), it->second ) )
		{
			err_f.push_back( it->first );
			final = false;
		}
	}
	return final;
}

void ResourceManager::CreateShaderObject( const char* name, const char* source, GLSLShaderType type )
{
	if ( ShaderObjectExists( name ) )
	{
		Terminal.Warning( "Shader " + string(name) + " already exists." );
		return;
	}
	
	ShaderObject* shader_obj = new ShaderObject();
	shader_obj->shaderType = GetGLShaderEnum(type);
	InsertShaderObject( name, shader_obj );
	CreateShader(shader_obj->shader, shader_obj->shaderType, source );
}

void ResourceManager::CreateShaderProgram( const char* name, const char* shaders_objects[], int32 count )
{
	if ( ShaderProgramExists( name ) )
	{
		Terminal.Warning("Program " + string(name) + " already exists.");
		return;
	}
	
	ShaderProgram* prog = new ShaderProgram();
	prog->name = name;
	InsertShaderProgram( name, prog);
	uint32 i;
	uint32* shaders = new uint32[count];
	for ( i = 0; i < count; ++i )
	{
		ShaderObject* obj = GetShaderObject( shaders_objects[i]);
		prog->shaders.push_back(  obj );
		shaders[i] = prog->shaders[i]->shader;
		obj->program = prog;
	}
	CreateProgram( prog->program, shaders, count);
	for ( i = 0; i < count; ++i )
	{
		if ( HasSubroutines( prog->program, prog->shaders[i]->shaderType ) )
		{
			SetSubroutineUniformLocations( prog->program, prog->shaders[i]->shaderType, prog->shaders[i]->subroutine );
		}
	}
	delete[] shaders;
}

void ResourceManager::UpdateShaderProgram( const char* name, GLSLShaderType type, const char* source )
{
	ShaderObject* oldObject = GetShaderObject( name );
	ShaderProgram* prog = GetShaderProgram( oldObject->program->name.c_str() );

	
	ShaderObject newObject;
	if (type == GLSLShaderType::UNKNOWN)
	{
		newObject.shaderType = oldObject->shaderType;
	}
	else
	{
		newObject.shaderType = GetGLShaderEnum(type);
	}
	CreateShader( newObject.shader, newObject.shaderType, source );

	uint32 i;
	uint32 count = static_cast<uint32>(prog->shaders.size());
	uint32* shaders = new uint32[count];
	for ( i = 0; i < count; ++i )
	{
		if ( IsAttached(prog->program, prog->shaders[i]->shader) )
		{
			DetachShader( prog->program, prog->shaders[i]->shader );
		}
	}
	DeleteShaderObject( oldObject->shader );
	oldObject->shader = newObject.shader;
	for ( i = 0; i < count; ++i )
	{
		shaders[i] = prog->shaders[i]->shader;
	}
	RemakeProgram(prog->program, shaders, count);
	delete[] shaders;
}

void ResourceManager::InsertShaderObject( const char* name, ShaderObject* data )
{
	shaderObjects.insert( std::make_pair( string(name), data ) );
}

void ResourceManager::InsertShaderProgram( const char* name, ShaderProgram* data )
{
	shaderPrograms.insert( std::make_pair( string(name), data ) );
}

ShaderObject* ResourceManager::GetShaderObject( const char* name ) const
{
	if ( !ShaderObjectExists( name ) )
	{
		Terminal.Warning( "Shader Object: " + string(name) + " does not exist." );
		return NULL;
	}
	return shaderObjects.at(name);

}

ShaderProgram* ResourceManager::GetShaderProgram( const char* name ) const
{
	if ( !ShaderProgramExists( name ) )
	{
		Terminal.Warning( "Shader Program: " + string(name) + " does not exist." );
		return NULL;
	}
	return shaderPrograms.at(name);
}

bool ResourceManager::ShaderObjectExists( const char* name ) const
{
	ShaderObjects::const_iterator it_shader = shaderObjects.find(name);
	if ( it_shader != shaderObjects.end() )
	{
		// Shader exists
		return true;
	}
	// Shader does not exists
	return false;
}

bool ResourceManager::ShaderProgramExists( const char* name ) const
{
	ShaderPrograms::const_iterator it_program = shaderPrograms.find(name);
	if ( it_program != shaderPrograms.end() )
	{
		// Program exists
		return true;
	}
	// Program does not exists
	return false;
}