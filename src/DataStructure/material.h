#ifndef material_h
#define material_h

#include <vector>
#include "../DataStructure/shader.h"

class Material
{
	public:
	
		Material();
		~Material() {}
	
		Shader* shader;
		unsigned int diffuseTextureId;
		unsigned int specularTextureId;
		unsigned int normalTextureId;
	
		void SetShader( Shader* shader );
		void UseMaterial();
		void SetDiffuseTexture( const char* image );
		void SetSpecularTexture( const char* image );
		void SetNormalTexture( const char* image );
	
		void SetUniforms();
	
		std::string name;
		int wireframe_enabled;
		int two_sided;
		unsigned int textureCount;
		float shininess;
		float shininess_strenght;
		float opacity;
		float bump_scaling;
		float diffuse[4];
		float ambient[4];
		float specular[4];
		float emissive[4];
		float transparent[4];
		float reflective[4];
	
	private:
		
	
};


#endif /* material_hp */
