#ifndef material_h
#define material_h

#include "../DataStructure/shader.h"

class Material
{
	public:
	
		Material( Shader* shader = nullptr );
		~Material();
	
		Shader* shader;
		unsigned int texture;
	
		void SetShader( Shader* shader );
		void SetTexture( unsigned int texture );
	
	private:
	
	
};


#endif /* material_hp */
