#ifndef component_h
#define component_h

enum class ComponentTypes
{
    UNKNOWN = 0,
    TRANSFORM = 1,
    CAMERA,
	MESH,
	MESH_RENDERER,
    LIGHT
};

class Entity;
class Component
{
	public:
		Entity* entity;
};

#endif /* component_h */