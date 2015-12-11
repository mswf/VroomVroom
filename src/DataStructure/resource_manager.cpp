#include "resource_manager.h"
#include "data_types.h"
#include "texture.h"
#include "mesh.h"
#include "material.h"
#include "shader.h"
#include "../content.h"
#include "../console.h"
#include "../Utilities/helperFunctions.h"

unsigned int ResourceManager::materialId = 0;

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
	bool vertexDefault = ImportShader( "shaders/default_vert.glsl", GLSLShaderType::VERTEX );
	bool fragmetDefault = ImportShader( "shaders/default_frag.glsl", GLSLShaderType::FRAGMENT );
	if ( !(vertexDefault && fragmetDefault) )
	{
		Terminal.Warning("Default shaders were missing. Loading builtin shaders.");
		LoadBuiltinShader();
	}
	else
	{
		const char* shaders[2] = { "shaders/default_vert.glsl", "shaders/default_frag.glsl" };
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
	CreateShaderObject( "shaders/default_vert.glsl", builtin_vertex, GLSLShaderType::VERTEX );
	CreateShaderObject( "shaders/default_frag.glsl", builtin_fragment, GLSLShaderType::FRAGMENT );
	const char* shaders[2] = { "shaders/default_vert.glsl", "shaders/default_frag.glsl" };
	CreateShaderProgram( "Default", shaders, 2);
}

// Meshes

Mesh* ResourceManager::GetMesh( const char* name ) const
{
	if ( !MeshExists( name ) )
	{
		Terminal.Warning( std::string("Mesh: " + std::string(name) + " not imported.") );
		return NULL;
	}
	return meshes.at(name);
}

bool ResourceManager::ImportMesh( const char* name )
{
	return imp.ImportObjFile( name );
}

bool ResourceManager::ImportMesh( const std::vector< std::string >& files, std::vector< std::string >& err_f )
{
	bool final = true;
	std::vector< std::string >::const_iterator iter = files.begin();
	std::vector< std::string >::const_iterator end = files.end();
	for ( ; iter != end; ++iter )
	{
		if ( !imp.ImportObjFile( (*iter) ) )
		{
			std::string error( *iter );
			err_f.push_back( error );
			final = false;
		}
	}
	return final;
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
	meshes.insert( std::pair< std::string, Mesh* >( std::string(name), data ) );
}

bool ResourceManager::MeshExists( const char* name ) const
{
	std::map< std::string, Mesh* >::const_iterator iter_mesh = meshes.find(name);
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
	std::map< std::string, ModelInstance* >::const_iterator iter_model = models.find(name);
	std::map< std::string, Mesh* >::const_iterator iter_mesh = meshes.find(name);
	
	std::string temp_name(name);
	if ( iter_model == models.end() )
	{
		if ( iter_mesh != meshes.end() && !(*iter_mesh).second->isBuffered)
		{
			//std::string warning_msg( temp_name + " has not been buffered yet." );
			//Terminal.Log( warning_msg );
			
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
			std::string err_msg( "No mesh by the name of" + temp_name + " has been found in resources." );
			Terminal.Warning( err_msg );
			return NULL;
		}
	}
	return (*iter_model).second;
}

void ResourceManager::InsertModelInstance( const char* name, ModelInstance* instance )
{
	models.insert( std::pair< std::string, ModelInstance* >( std::string(name), instance ) );
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

bool ResourceManager::ImportImage( const std::vector< std::string >& files, std::vector< std::string >& err_f, bool vertical_flip )
{
	bool final = true;
	std::vector< std::string >::const_iterator iter = files.begin();
	std::vector< std::string >::const_iterator end = files.end();
	for ( ; iter != end; ++iter )
	{
		if ( !ImportImage( (*iter).c_str(), vertical_flip ) )
		{
			std::string error( *iter );
			err_f.push_back( error );
			final = false;
		}
	}
	return final;
}

void ResourceManager::InsertImage( const char* name, ImageData* data )
{
	images.insert( std::pair< std::string, ImageData* >( std::string(name), data ) );
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
			Terminal.Warning( std::string("Image not buffered, returning 0." ) );
			return 0;
		}
	}
	return imageIds.at(name);
}

ImageData* ResourceManager::GetImageData( const char* name ) const
{
	if ( !ImageExists( name ) )
	{
		Terminal.Warning( std::string("Image: " + std::string(name) + " not imported.") );
		return NULL;
	}
	return images.at(name);
}

bool ResourceManager::BufferImage1D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL) return false;
	std::map< std::string, unsigned int >::const_iterator iter_imageId = imageIds.find(name);
	
	if ( !img->isBuffered || iter_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture1D( GL_RGBA, img->width, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, false );
		img->isBuffered = true;
		img->mipmapping = false;
		imageIds.insert( std::pair< std::string, unsigned int >( std::string(name), img->imageId ) );
	}
	return true;
}

bool ResourceManager::BufferImage2D( const char* name )
{
	ImageData* img = GetImageData( name );
	if (img == NULL) return false;
	std::map< std::string, unsigned int >::const_iterator iter_imageId = imageIds.find(name);
	
	if ( !img->isBuffered || iter_imageId == imageIds.end() )
	{
		img->imageId = BufferTexture2D( GL_RGBA, img->width, img->height, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, false, true, false);
		img->isBuffered = true;
		img->mipmapping = true;
		imageIds.insert( std::pair< std::string, unsigned int >( std::string(name), img->imageId ) );
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
	std::map< std::string, unsigned int >::const_iterator iter_imageId = imageIds.find(name);
	if ( img->isBuffered || iter_imageId != imageIds.end() )
	{
		UpdateBufferImage2D( img->imageId, 0, 0, img->width, img->width, GL_RGBA, GL_UNSIGNED_BYTE, img->pixelData, img->mipmapping );
		return true;
	}
	return false;
}

unsigned int ResourceManager::CreateCubeMap( const std::vector< std::pair< unsigned char*, unsigned int > >* textures, int width, int height )
{
	unsigned int cubeMapId;
	glGenTextures( 1, &cubeMapId );
	
	std::vector< std::pair< unsigned char*, GLenum > >::const_iterator iter = textures->begin();
	std::vector< std::pair< unsigned char*, GLenum > >::const_iterator end = textures->end();
	for ( ; iter != end; ++iter )
	{
		BufferTextureCubeMap(cubeMapId, (*iter).second, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (*iter).first);
	}
	return cubeMapId;
}


bool ResourceManager::ImageExists( const char* name ) const
{
	std::map< std::string, ImageData* >::const_iterator iter_image = images.find(name);
	if ( iter_image != images.end() )
	{
		// Image exists
		return true;
	}
	// Image does not exists
	return false;
}

// Materials

Material* ResourceManager::GetMaterialById( unsigned int materialId ) const
{
// TODO(Valentinas): Add a check if it exists
	return materials.at( materialId );
}

Material* ResourceManager::GetMaterialByName( const char* name ) const
{
	return materials.at( GetMaterialId(name) );
}

unsigned int ResourceManager::GetMaterialId( const char* name ) const
{
	if ( !MaterialExists(name) )
	{
		Terminal.Warning( std::string("Material: " + std::string(name) + " not found." ) );
		return 0;
	}
	return materialIds.at( name );
}

void ResourceManager::InsertMaterial( unsigned int id, const char* name, Material* data )
{
	if ( !MaterialExists(name) )
	{
		materialIds.insert( std::pair< std::string, unsigned int >( std::string(name), id ) );
		materials.insert( std::pair< unsigned int, Material* >( id, data ) );
	}
}

bool ResourceManager::MaterialExists( const char* name ) const
{
	std::map< std::string, unsigned int >::const_iterator iter_mtl = materialIds.find(name);
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
	std::string path(Content::GetPath() + "/" + name);
	if ( HelperFunctions::FileExists( path.c_str() ) )
	{
		// Get source
		std::string source = HelperFunctions::ReadFile( path );
		
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
		Terminal.Log( std::string("Reimporting shader: ") + name, true );
		std::string source = HelperFunctions::ReadFile( path );
		UpdateShaderProgram( name, type, source.c_str() );
		return true;
	}
	return false;
}


bool ResourceManager::ImportShader( const std::vector< std::pair< std::string, GLSLShaderType > >& list,
								    std::vector< std::string >& err_f )
{
	bool final = true;
	std::vector< std::pair< std::string, GLSLShaderType > >::const_iterator iter = list.begin();
	std::vector< std::pair< std::string, GLSLShaderType > >::const_iterator iter_end = list.end();
	for ( ; iter != iter_end; ++iter )
	{
		if ( !ImportShader( (*iter).first.c_str(), (*iter).second ) )
		{
			std::string error( (*iter).first );
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

void ResourceManager::CreateShaderProgram( const char* name, const char* shaders_objects[], int count )
{
	ShaderProgram* prog = new ShaderProgram();
	prog->name = name;
	ResourceManager::getInstance().InsertShaderProgram( name, prog);
	int i;
	GLuint* shaders = new GLuint[count];
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
			SetSubroutineUniformLocations( prog->program, *prog->shaders[i] );
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
	
	int i;
	int count = (int)p->shaders.size();
	GLuint* shaders = new GLuint[count];
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
		shaderObjects.insert( std::pair< std::string, ShaderObject* >( std::string(name), data ) );
		return;
	}
	Terminal.Warning("Shader " + std::string(name) + " already exists." );
}

void ResourceManager::InsertShaderProgram( const char* name, ShaderProgram* data )
{
	if ( !ShaderProgramExists( name ) )
	{
		shaderPrograms.insert( std::pair< std::string, ShaderProgram* >( std::string(name), data ) );
		return;
	}
	Terminal.Warning("Program " + std::string(name) + " already exists." );
}

ShaderObject* ResourceManager::GetShaderObject( const char* name ) const
{
	if ( !ShaderObjectExists( name ) )
	{
		Terminal.Warning( std::string("Shader Object: " + std::string(name) + " does not exist.") );
		return NULL;
	}
	return shaderObjects.at(name);

}

ShaderProgram* ResourceManager::GetShaderProgram( const char* name ) const
{
	if ( !ShaderProgramExists( name ) )
	{
		Terminal.Warning( std::string("Shader Program: " + std::string(name) + " does not exist.") );
		return NULL;
	}
	return shaderPrograms.at(name);
}

bool ResourceManager::ShaderObjectExists( const char* name ) const
{
	std::map< std::string, ShaderObject* >::const_iterator iter_shader = shaderObjects.find(name);
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
	std::map< std::string, ShaderProgram* >::const_iterator iter_program = shaderPrograms.find(name);
	if ( iter_program != shaderPrograms.end() )
	{
		// Program exists
		return true;
	}
	// Program does not exists
	return false;
}