#ifndef cMaterial_h
#define cMaterial_h

#include "../DataStructure/shader.h"
#include "../Components/component.h"

class CMaterial : public Component
{
	public:
	
		static const int familyId;
	
		CMaterial( Shader* shader = nullptr );
		~CMaterial();
	
		Shader* shader;
		
	private:
	
	
};


#endif /* cMaterial_hp */
