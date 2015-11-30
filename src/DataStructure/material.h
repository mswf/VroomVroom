#ifndef material_h
#define material_h

#include <string>

struct ShaderProgram;

class Material
{
	public:
	
		Material();
		~Material() {}
	
		ShaderProgram* shader;
		unsigned int diffuseTextureId;
		unsigned int specularTextureId;
		unsigned int normalTextureId;
	
		void SetShader( ShaderProgram* shader );
		void UseMaterial() const;
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
