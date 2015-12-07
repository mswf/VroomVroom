#ifndef material_h
#define material_h

#include <string>
#include "../glm/detail/type_vec4.hpp"

struct ShaderProgram;

class Material
{
	public:

		Material();
		~Material() {}
		static void SetArray(float* array, glm::vec4 values);

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

		void SetShininess( float sh );
		void SetShininessStrength( float str );
		void SetOpacity( float op );
		void SetBumpScaling( float bump );
		void SetAmbientColor( glm::vec4 colour );
		void SetDiffuseColor( glm::vec4 colour );
		void SetSpecularColor( glm::vec4 colour );
		void SetEmissiveColor( glm::vec4 colour );
		void SetTransparentColor( glm::vec4 colour );
		void SetReflectiveColor( glm::vec4 colour );
	
		const float& GetShininess() const;
		const float& GetShininessStrength() const;
		const float& GetOpacity() const;
		const float& GetBumpScaling() const;
		const glm::vec4 GetAmbientColor() const;
		const glm::vec4 GetDiffuseColor() const;
		const glm::vec4 GetSpecularColor() const;
		const glm::vec4 GetEmissiveColor() const;
		const glm::vec4 GetTransparentColor() const;
		const glm::vec4 GetReflectiveColor() const;
	
	
		std::string name;
		// TODO(Valentinas): Use int or bool?
		int wireframe_enabled;
		int two_sided;
		unsigned int textureCount;
		float shininess;
		float shininess_strenght;
		float opacity;
		float bump_scaling;
		float ambient[4];
		float diffuse[4];
		float specular[4];
		float emissive[4];
		float transparent[4];
		float reflective[4];

	private:


};


#endif /* material_hp */
