#ifndef entitySystem_h
#define entitySystem_h

#include "../Patterns/singleton.h"

class EntitySystem : public Singleton<EntitySystem>
{
	public:
		EntitySystem();
		~EntitySystem();
		
		void Init();
		void Main();
		void Update(float);
	
};

#endif /* entitySystem_hpp */
