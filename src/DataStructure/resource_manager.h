#ifndef resource_manager_h
#define resource_manager_h

#include <string>
#include <map>
#include <vector>
#include "../Patterns/singleton.h"
#include "../IO/importer.h"

class Material;

struct Mesh;
struct ModelInstance;
struct ImageData;
struct ShaderObject;
struct ShaderProgram;

enum class GLSLShaderType;

class ResourceManager : public Singleton<ResourceManager>
{
	public:
	
		ResourceManager();
		~ResourceManager();
	
		// Meshes
		bool ImportMesh( const char* name );
		bool ImportMesh( const std::vector<std::string>& list, std::vector< std::string >& errors );
		void UpdateMeshBuffer();
		void InsertMesh( const char* name, Mesh* data );
		bool MeshExists( const char* name ) const;

		// Model Instances
		ModelInstance* GetModel( const char* name );
		void InsertModelInstance( const char* name, ModelInstance* instance );
	
		// Images
		ImageData* GetImageData( const char* name ) const;
		unsigned int GetImageId( const char* name );
		bool ImportImage( const char* name, bool vertical_flip = true );
		bool ImportImage( const std::vector< std::string >& list, std::vector< std::string >& err_f, bool vertical_flip = true );
		bool ReImportImage( const char* name, bool vertical_flip = true );
		bool BufferImage1D( const char* name );
		bool BufferImage2D( const char* name );
		bool BufferImage3D( const char* name );
		bool UpdateImage2DBuffer( const char* name );
		void InsertImage( const char* name, ImageData* data );
		bool ImageExists( const char* name ) const;
	
		unsigned int CreateCubeMap( const std::vector< std::pair< unsigned char*, unsigned int > >* textures, int width, int height );
	
		// Materials
		static unsigned int materialId;
		Material* GetMaterialByName( const char* name ) const;
		Material* GetMaterialById( unsigned int materialId ) const;
		void InsertMaterial( unsigned int id, const char* name, Material* data );
		bool MaterialExists( const char* name ) const;
	
		// Shaders
		ShaderProgram* GetShaderProgram( const char* name ) const;
		ShaderObject* GetShaderObject( const char* name ) const;
		bool ImportShader( const char* name, GLSLShaderType type );
		bool ImportShader( const std::vector< std::pair< std::string, GLSLShaderType > >& list, std::vector< std::string >& err_f );
		bool ReImportShader( const char* name, GLSLShaderType type );
		void CreateShaderProgram( const char* name, const char* shaders_objects[], int count );
		void UpdateShaderProgram( const char* name, GLSLShaderType type, const char* source );
	
		void InsertShaderObject( const char* name, ShaderObject* data );
		void InsertShaderProgram( const char* name, ShaderProgram* data );
		bool ShaderObjectExists( const char* name ) const;
		bool ShaderProgramExists( const char* name ) const;
	
		// Audio
		bool ImportAudioFile( const char* name );
		bool ImportAudioFile( const std::vector<std::string>& list );
	
		void Initialize();
		void LoadBuiltinShader();
	
	private:
	
		unsigned int GetMaterialId( const char* name ) const;
	
	
		Importer imp;
	
		// Mesh information
		std::map< std::string, ModelInstance* > models;
		std::map< std::string, Mesh* > meshes;
	
		// Image information
		std::map< std::string, ImageData* > images;
		std::map< std::string, unsigned int > imageIds;
	
		// Material information
		std::map< unsigned int, Material* > materials;
		std::map< std::string, unsigned int > materialIds;
	
		// Shader information
		std::map< std::string, ShaderProgram* > shaderPrograms;
		std::map< std::string, ShaderObject* > shaderObjects;
};

#endif /* resource_manager_h */

