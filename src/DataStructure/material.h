#ifndef material_h
#define material_h

#include <string>
#include <glm/detail/type_vec4.hpp>

struct ShaderProgram;

class Material
{
	public:

		Material();
		~Material() {}
		void InitArray(float* array, glm::vec4 values);

		ShaderProgram* shader;
		unsigned int diffuseTextureId;
		unsigned int specularTextureId;
		unsigned int normalTextureId;
		unsigned int cubemapTextureId;
		unsigned int heightTextureId;

		void SetShader( ShaderProgram* shader );
		void UseMaterial() const;
		void SetUniforms();
		void SetDiffuseTexture( const char* name );
		void SetSpecularTexture( const char* name );
		void SetNormalTexture( const char* name );
		void SetCubeMapTexture( const char* name );
		void SetHeightMapTexture( const char* name );
		inline void IsDrawingWireframe( bool enabled )
		{
			wireframe_enabled = enabled;
		}
		inline void IsTwoSided( bool enabled )
		{
			two_sided = enabled;
		}

		std::string name;
		// TODO(Valentinas): Use int or bool?
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
