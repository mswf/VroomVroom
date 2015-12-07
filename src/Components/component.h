#ifndef component_h
#define component_h

enum class ComponentTypes
{
    UNKNOWN = 0,
    TRANSFORM = 1,
    CAMERA,
	MESH,
	MESH_RENDERER,
	DEBUG_RENDERER,
    LIGHT,
	LUA
};

class Entity;
class Component
{
	public:
		Entity* entity;
		virtual void Call() = 0;
		inline void IncrementAddedToEntity() { ++numberOfTimesAdded; }
	private:
		short numberOfTimesAdded = 0;
};

#endif /* component_h */