#ifndef resource_manager_h
#define resource_manager_h

#include <string>
#include <map>
#include "data_types.h"
#include "../Patterns/singleton.h"
#include "../IO/importer.h"

struct ImageData
{
	int imageId;
	unsigned char* pixelData;
	unsigned int width;
 	unsigned int height;
	unsigned int components;
	bool isBuffered;
	bool hasBufferChanged;
	ImageData() :
		imageId(-1),
		//pixelData(NULL),
		width(0), height(0), components(0),
		isBuffered(false), hasBufferChanged(false)
	{}
};

class Material;

class ResourceManager : public Singleton<ResourceManager>
{
	public:
	
		ResourceManager();
		~ResourceManager();
	
		static unsigned int materialId;
	
		ModelInstance* GetModel( const char* name );
		Material* GetMaterialByName( const char* name ) const;
		Material* GetMaterialById( unsigned int materialId ) const;
		ImageData* GetImageData( const char* name ) const;
		unsigned int GetImageId( const char* name );
	
		bool ImportMesh( const char* name );
		bool ImportImage( const char* name );
		bool ImportShader( const char* name );
		bool ImportAudioFile( const char* name );
		bool ImportMesh( const std::vector<std::string>& list, std::vector< std::string >& errors );
		bool ImportImage( const std::vector<std::string>& list, std::vector< std::string >& err_f );
		bool ImportShader( const std::vector<std::string>& list );
		bool ImportAudioFile( const std::vector<std::string>& list );
	
		bool BufferImage1D( const char* name );
		bool BufferImage2D( const char* name );
		bool BufferImage3D( const char* name );
		void UpdateMeshBuffer();
	
		bool MeshExists( const char* name );
		bool ImageExists( const char* name );
	
		void InsertModelInstance( const char* name, ModelInstance* instance );
		void InsertMesh( const char* name, Mesh* data );
		void InsertMaterial( unsigned int id, const char* name, Material* data );
		void InsertImage( const char* name, ImageData* data );
	
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
		std::map< std::string, ShaderProgram > shaderPrograms;
		std::map< std::string, ShaderObject > shaderObjects;
		std::map< std::string, std::string > shaderSources;
};

#endif /* resource_manager_h */

