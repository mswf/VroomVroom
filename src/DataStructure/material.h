#ifndef material_h
#define material_h

#include "../DataStructure/shader.h"
#include "../Components/component.h"

class Material
{
	public:
	
		Material( Shader* shader = nullptr );
		~Material();
	
		Shader* shader;
		
	private:
	
	
};


#endif /* material_hp */
