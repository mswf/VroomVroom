#ifndef componentsystem_h
#define componentsystem_h

class ComponentSystem
{
	public:
		ComponentSystem() {};
		virtual ~ComponentSystem() {};
        virtual bool Initialize() { printf(" ComponentSystem Initialization() not overriden! \n"); return false; };
		virtual void Update(void* data) { printf(" ComponentSystem Update() not overriden! \n"); };
		virtual void SendMessage(void* data) { printf(" ComponentSystem SendMessage() not overriden! \n"); };
};

#endif