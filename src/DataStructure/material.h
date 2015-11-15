#ifndef material_h
#define material_h

#include "../DataStructure/shader.h"

class Material
{
	public:
	
		Material( Shader* shader = nullptr );
		~Material();
	
		Shader* shader;
		unsigned int diff_texture;
		unsigned int normal_texture;
	
		void SetShader( Shader* shader );
		void SetDiffuse( unsigned int texture );
		void SetNormal( unsigned int texture );
	
	private:
	
	
};


#endif /* material_hp */
