#ifndef material_h
#define material_h

#include <string>
#include "../glm/detail/type_vec4.hpp"


struct ShaderProgram;
struct Uniform;

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
	
		void UseMaterial() const;
		void SetUniforms() const;
		Material* Clone() const { return new Material(*this); }
	
		inline void SetDrawingWireframe( bool enabled ) { wireframe_enabled = enabled; }
		inline void SetTwoSided( bool enabled ) { two_sided = enabled; }
		inline bool GetDrawingWireframe() { return wireframe_enabled; }
		inline bool GetTwoSided() { return two_sided; }
	
		inline void SetShader( ShaderProgram* shader ) { this->shader = shader; }
		
		void SetDiffuseTexture( const char* name );
		void SetSpecularTexture( const char* name );
		void SetNormalTexture( const char* name );
		void SetCubeMapTexture( const char* name );
		void SetHeightMapTexture( const char* name );

		inline void SetShininess( float sh ) { shininess = sh; }
		inline void SetShininessStrength( float str ) { shininess_strenght = str; }
		inline void SetOpacity( float op ) { opacity = op; }
		inline void SetBumpScaling( float bump ) { bump_scaling = bump; }
		void SetAmbientColor( glm::vec4 colour );
		void SetDiffuseColor( glm::vec4 colour );
		void SetSpecularColor( glm::vec4 colour );
		void SetEmissiveColor( glm::vec4 colour );
		void SetTransparentColor( glm::vec4 colour );
		void SetReflectiveColor( glm::vec4 colour );
	
		inline const float& GetShininess() const { return shininess; }
		inline const float& GetShininessStrength() const { return shininess_strenght; }
		inline const float& GetOpacity() const { return opacity; }
		inline const float& GetBumpScaling() const { return bump_scaling; }
		const glm::vec4 GetAmbientColor() const;
		const glm::vec4 GetDiffuseColor() const;
		const glm::vec4 GetSpecularColor() const;
		const glm::vec4 GetEmissiveColor() const;
		const glm::vec4 GetTransparentColor() const;
		const glm::vec4 GetReflectiveColor() const;
	
		std::string name;
		bool wireframe_enabled;
		bool two_sided;
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

		void SetShaderUniform( Uniform u ) const;
};


#endif /* material_hp */
