#ifndef resource_manager_h
#define resource_manager_h

#include "../Utilities/typedef.h"
#include <string>
#include <map>
#include <vector>
#include "../Patterns/singleton.h"
#include "../IO/importer.h"

#define Assets ResourceManager::getInstance()

class Material;

struct Mesh;
struct ModelInstance;
struct ImageData;
struct ShaderObject;
struct ShaderProgram;

enum class GLSLShaderType;

typedef std::map< string, ModelInstance* > Models;
typedef std::map< string, Mesh* > Meshes;
typedef std::map< string, ImageData* > Images;
typedef std::map< uint32, Material* > Materials;
typedef std::map< string, ShaderObject* > ShaderObjects;
typedef std::map< string, ShaderProgram* > ShaderPrograms;
typedef std::map< string, uint32 > StringToID;
typedef std::vector< string > StringVector;


class ResourceManager : public Singleton<ResourceManager>
{
	public:

		ResourceManager();
		~ResourceManager();

		// Meshes
		Mesh* GetMesh( const char* name ) const;
		bool ImportMesh( const char* name );
		bool ImportMesh( const StringVector& list );
		void UpdateMeshBuffer();
		void InsertMesh( const char* name, Mesh* data );
		void SetMeshScale( const char* name, float scale );
		bool MeshExists( const char* name ) const;
		void MergeToExistingMesh( const char* name, Mesh* data );

		// Model Instances
		ModelInstance* GetModel( const char* name );
		void InsertModelInstance( const char* name, ModelInstance* instance );

		// Images
		ImageData* GetImageData( const char* name ) const;
		uint32 GetImageId( const char* name );
		bool ImportImage( const char* name, bool vertical_flip = true );
		bool ImportImage( const StringVector& list, bool vertical_flip = true );
		bool ReImportImage( const char* name, bool vertical_flip = true );
		bool BufferImage1D( const char* name );
		bool BufferImage2D( const char* name );
		bool BufferImage3D( const char* name );
		bool UpdateImage2DBuffer( const char* name );
		void InsertImage( const char* name, ImageData* data );
		bool ImageExists( const char* name ) const;

		// Cube Maps
		uint32 GetCubeMapId( const char* name );
		bool ImportCubeMap( const char* textures[], const char* name, bool mipmap );
		void InsertCubeMap( const char* name, uint32 id );
		bool CubeMapExists( const char* name ) const;
	
		// Materials
		static uint32 materialId;
		Material* GetMaterialByName( const char* name ) const;
		Material* GetMaterialById( uint32 materialId ) const;
		void InsertMaterial( uint32 id, const char* name, Material* data );
		bool MaterialExists( const char* name ) const;
		bool MaterialExists( const uint32 id ) const;
		void ClearMaterials();

		// Shaders
		ShaderProgram* GetShaderProgram( const char* name ) const;
		ShaderObject* GetShaderObject( const char* name ) const;
		bool ImportShader( const char* name, GLSLShaderType type );
		bool ImportShader( const std::vector< std::pair< string, GLSLShaderType > >& list );
		bool ReImportShader( const char* name, GLSLShaderType type );
		void CreateShaderObject( const char* name, const char* source, GLSLShaderType type );
		void CreateShaderProgram( const char* name, const char* shaders_objects[], int count );
		void UpdateShaderProgram( const char* name, GLSLShaderType type, const char* source );

		bool ImportAndCreateShader( const char* vertex, const char* fragment, const char* program_name );
	
		void InsertShaderObject( const char* name, ShaderObject* data );
		void InsertShaderProgram( const char* name, ShaderProgram* data );
		bool ShaderObjectExists( const char* name ) const;
		bool ShaderProgramExists( const char* name ) const;

		// Audio
		bool ImportAudioFile( const char* name );
		bool ImportAudioFile( const StringVector& list );

		void Initialize();
		void LoadBuiltinShader();
		void LoadDefaultMeshes();

	private:

		uint32 GetMaterialId( const char* name ) const;


		Importer importer;

		// Mesh information
		Models models;
		Meshes meshes;

		// Image information
		Images images;
		StringToID imageIds;
		StringToID cubeMapIds;

		// Material information
		Materials materials;
		StringToID materialIds;

		// Shader information
		ShaderPrograms shaderPrograms;
		ShaderObjects shaderObjects;
};

#endif /* resource_manager_h */

