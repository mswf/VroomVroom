#pragma once
class ComponentSystem
{
	public:
		ComponentSystem() {};
		virtual ~ComponentSystem() {};
		virtual void Initialize() {};
		virtual void Update(void* data) {};
		virtual void SendMessage(void* data) {};
};

