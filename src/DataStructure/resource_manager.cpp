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


ResourceManager::ResourceManager() {}
//TODO(Valentinas): Reload meshes
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
	bool fragmetDefault = ImportShader( DEFAULT_FRAGMENT, GLSLShaderType::FRAGMENT );
	if ( !(vertexDefault && fragmetDefault) )
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
	return imp.ImportObjFile( name );
}

bool ResourceManager::ImportMesh( const std::vector< string >& files, std::vector< string >& err_f )
{
	bool final = true;
	std::vector< string >::const_iterator iter = files.begin();
	std::vector< string >::const_iterator end = files.end();
	for ( ; iter != end; ++iter )
	{
		if ( !imp.ImportObjFile( (*iter) ) )
		{
			string error( *iter );
			err_f.push_back( error );
			final = false;
		}
	}
	return final;
}

void ResourceManager::UpdateMeshBuffer()
{
	std::map< string, Mesh* >::const_iterator iter = meshes.begin();
	std::map< string, Mesh* >::const_iterator end = meshes.end();
	for ( ; iter != end; ++iter )
	{
		if ( (*iter).second->hasBufferChanged )
		{
			printf( "Changed %s", (*iter).first.c_str() );
		}
	}
}

// DON'T USE THIS FUNCTION< NOT WORKING PROPERLY
void ResourceManager::MergeToExistingMesh( const char* name, Mesh* data )
{
	Mesh* m = GetMesh(name);
	if ( m->hasPositions && data->hasPositions )
	{
		m->vertices.insert(  std::end( m->vertices ), std::begin( data->vertices ), std::end( data->vertices ) );
		m->indices.insert(  std::end( m->indices ), std::begin( data->indices ), std::end( data->indices ) );
		m->numIndices += data->numIndices;
	}
	if ( m->hasNormals && data->hasNormals )
	{
		m->normals.insert(  std::end( m->normals ), std::begin( data->normals ), std::end( data->normals ) );
	}
	if ( m->hasUVs && data->hasUVs )
	{
		m->uvs.insert(  std::end( m->uvs ), std::begin( data->uvs ), std::end( data->uvs ) );
	}
	if ( m->hasTangentsAndBitangets && data->hasTangentsAndBitangets )
	{
		m->tangents.insert(  std::end( m->tangents ), std::begin( data->tangents ), std::end( data->tangents ) );
		m->bitangents.insert(  std::end( m->bitangents ), std::begin( data->bitangents ), std::end( data->bitangents ) );
	}
}

void ResourceManager::SetMeshScale( const char* name, float scale )
{
	Mesh* m = GetMesh(name);
	if ( m->hasPositions )
	{
		m->scaleFactor = scale;
		auto it = m->vertices.begin();
		auto end = m->vertices.end();
		for ( ; it != end; ++it)
		{
			(*it) *= scale;
		}
	}
}

void ResourceManager::InsertMesh( const char* name, Mesh* data )
{
	meshes.insert( std::pair< string, Mesh* >( string(name), data ) );
}

bool ResourceManager::MeshExists( const char* name ) const
{
	std::map< string, Mesh* >::const_iterator iter_mesh = meshes.find(name);
	if ( iter_mesh != meshes.end() )
	{
		// Mesh exists
		return true;
	}
	// Mesh does not exists
	return false;
}

// Model Instances

ModelInstance* ResourceManager::GetModel( const char* name )
{
	std::map< string, ModelInstance* >::const_iterator iter_model = models.find(name);
	std::map< string, Mesh* >::const_iterator iter_mesh = meshes.find(name);
	
	if ( iter_model == models.end() )
	{
		if ( iter_mesh != meshes.end() && !(*iter_mesh).second->isBuffered)
		{
			//Terminal.Log( string(name) + " has not been buffered yet." );
			
			//Buffer ModelInstance & add to resource list
			ModelInstance* newInstance = new ModelInstance();
			unsigned int mtl = (*iter_mesh).second->materialId;
			newInstance->materialId = mtl;
			BufferMesh( (*iter_mesh).second, newInstance );
			
			// Set the mesh's buffer to true
			(*iter_mesh).second->isBuffered = true;
			
			InsertModelInstance( name, newInstance );
			
			return GetModel( name );
		}
		
		// Checking if the mesh is imported
		if ( iter_mesh == meshes.end() )
		{
			Terminal.Warning( "No mesh by the name of" + string(name) + " has been found in resources." );
			return NULL;
		}
	}
	return (*iter_model).second;
}

void ResourceManager::InsertModelInstance( const char* name, ModelInstance* instance )
{
	models.insert( std::pair< string, ModelInstance* >( string(name), instance ) );
}

// Images

bool ResourceManager::ImportImage( const char* name, bool vertical_flip )
{
	if ( ImageExists(name) )
	{
		Terminal.Warning( "Image already imported. Aborting redundant loading" );
		return false;
	}
	return imp.ImportImage( name, vertical_flip );
}

bool ResourceManager::ReImportImage( const char* name, bool vertical_flip )
{
	if ( ImageExists(name) )
	{
		ImageData* data = imp.ReImportImage( name, vertical_flip );
		ImageData* old = GetImageData(name);
		// ???? !!!!! ????
		old->pixelData = data->pixelData;
		UpdateImage2DBuffer(name);
		return true;
	}
	Terminal.Warning( "Image not imported at all. Using incorrect function?" );
	return false;
}

bool ResourceManager::ImportImage( const std::vector< string >& files, std::vector< string >& err_f, bool vertical_flip )
{
	bool final = true;
	std::vector< string >::const_iterator iter = files.begin();
	std::vector< string >::const_iterator end = files.end();
	for ( ; iter != end; ++iter )
	{
		if ( !ImportImage( (*iter).c_str(), vertical_flip ) )
		{
			string error( *iter );
			err_f.push_back( error );
			final = false;
		}
	}
	return final;
}

void ResourceManager::InsertImage( const char* name, ImageData* data )
{
	images.insert( std::pair< string, ImageData* >( string(name), data ) );
}

uint32 ResourceManager::GetImageId( const char *name )
{
	std::map< string, unsigned int >::const_iterator iter_image = imageIds.find(name);
	if ( iter_image == imageIds.end() )
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
	if (img == NULL) return false;
	std::map< string, unsigned int >::const_iterator iter_imageId = imageIds.find(name);
	
	if ( !img->isBuffered || iter_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture1D( GL_RGBA, img->width, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, false );
		img->isBuffered = true;
		img->mipmapping = false;
		imageIds.insert( std::pair< string, uint32 >( string(name), img->imageId ) );
	}
	return true;
}

bool ResourceManager::BufferImage2D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL) return false;
	std::map< string, uint32 >::const_iterator iter_imageId = imageIds.find(name);
	
	if ( !img->isBuffered || iter_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture2D( GL_RGBA, img->width, img->height, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, false, true, false);
		img->isBuffered = true;
		img->mipmapping = true;
		imageIds.insert( std::pair< string, uint32 >( string(name), img->imageId ) );
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
	std::map< string, uint32 >::const_iterator iter_imageId = imageIds.find(name);
	if ( img->isBuffered || iter_imageId != imageIds.end() )
	{
		UpdateBufferImage2D( img->imageId, 0, 0, img->width, img->width, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, img->mipmapping );
		return true;
	}
	return false;
}

unsigned int ResourceManager::CreateCubeMap( const std::vector< std::pair< uint8*, uint32 > >* textures, int32 width, int32 height )
{
	uint32 cubeMapId;
	glGenTextures( 1, &cubeMapId );
	
	std::vector< std::pair< uint8*, GLenum > >::const_iterator iter = textures->begin();
	std::vector< std::pair< uint8*, GLenum > >::const_iterator end = textures->end();
	for ( ; iter != end; ++iter )
	{
		BufferTextureCubeMap(cubeMapId, (*iter).second, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (*iter).first);
	}
	return cubeMapId;
}


bool ResourceManager::ImageExists( const char* name ) const
{
	std::map< string, ImageData* >::const_iterator iter_image = images.find(name);
	if ( iter_image != images.end() )
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
// TODO(Valentinas): Add a check if it exists
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
		materialIds.insert( std::pair< string, uint32 >( string(name), id ) );
		materials.insert( std::pair< uint32, Material* >( id, data ) );
	}
}

bool ResourceManager::MaterialExists( const char* name ) const
{
	std::map< string, uint32 >::const_iterator iter_mtl = materialIds.find(name);
	if ( iter_mtl != materialIds.end() )
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

bool ResourceManager::ReImportShader( const char* name, GLSLShaderType type )
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


bool ResourceManager::ImportShader( const std::vector< std::pair< string, GLSLShaderType > >& list,
								    std::vector< string >& err_f )
{
	bool final = true;
	std::vector< std::pair< string, GLSLShaderType > >::const_iterator iter = list.begin();
	std::vector< std::pair< string, GLSLShaderType > >::const_iterator iter_end = list.end();
	for ( ; iter != iter_end; ++iter )
	{
		if ( !ImportShader( (*iter).first.c_str(), (*iter).second ) )
		{
			string error( (*iter).first );
			err_f.push_back( error );
			final = false;
		}
	}
	return final;
}

void ResourceManager::CreateShaderObject( const char* name, const char* source, GLSLShaderType type )
{
	ShaderObject* shader_obj = new ShaderObject();
	shader_obj->shaderType = GetGLShaderEnum(type);
	InsertShaderObject( name, shader_obj );
	CreateShader(shader_obj->shader, shader_obj->shaderType, source );
}

void ResourceManager::CreateShaderProgram( const char* name, const char* shaders_objects[], int32 count )
{
	ShaderProgram* prog = new ShaderProgram();
	prog->name = name;
	ResourceManager::getInstance().InsertShaderProgram( name, prog);
	int32 i;
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
	delete shaders;
}

void ResourceManager::UpdateShaderProgram( const char* name, GLSLShaderType type, const char* source )
{
	ShaderObject* _old = GetShaderObject( name );
	ShaderProgram* p = GetShaderProgram( _old->program->name.c_str() );
	
	ShaderObject _new;
	_new.shaderType = GetGLShaderEnum(type);
	CreateShader( _new.shader, _new.shaderType, source );
	
	uint32 i;
	uint32 count = (uint32)p->shaders.size();
	uint32* shaders = new uint32[count];
	for ( i = 0; i < count; ++i )
	{
		if ( IsAttached(p->program, p->shaders[i]->shader) )
		{
			DetachShader( p->program, p->shaders[i]->shader );
		}
	}
	DeleteShaderObject( _old->shader );
	_old->shader = _new.shader;
	for ( i = 0; i < count; ++i )
	{
		shaders[i] = p->shaders[i]->shader;
	}
	RemakeProgram(p->program, shaders, count);
}

void ResourceManager::InsertShaderObject( const char* name, ShaderObject* data )
{
	if ( !ShaderObjectExists( name ) )
	{
		shaderObjects.insert( std::pair< string, ShaderObject* >( string(name), data ) );
		return;
	}
	Terminal.Warning( "Shader " + string(name) + " already exists." );
}

void ResourceManager::InsertShaderProgram( const char* name, ShaderProgram* data )
{
	if ( !ShaderProgramExists( name ) )
	{
		shaderPrograms.insert( std::pair< string, ShaderProgram* >( string(name), data ) );
		return;
	}
	Terminal.Warning( "Program " + string(name) + " already exists." );
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
	std::map< string, ShaderObject* >::const_iterator iter_shader = shaderObjects.find(name);
	if ( iter_shader != shaderObjects.end() )
	{
		// Shader exists
		return true;
	}
	// Shader does not exists
	return false;
}

bool ResourceManager::ShaderProgramExists( const char* name ) const
{
	std::map< string, ShaderProgram* >::const_iterator iter_program = shaderPrograms.find(name);
	if ( iter_program != shaderPrograms.end() )
	{
		// Program exists
		return true;
	}
	// Program does not exists
	return false;
}