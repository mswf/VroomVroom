#ifndef material_h
#define material_h

#include "../DataStructure/shader.h"

class Material
{
	public:
	
		Material( Shader* shader = nullptr );
		~Material();
	
		Shader* shader;
	
		void SetShader( Shader* shader );
	
	private:
	
	
};


#endif /* material_hp */
