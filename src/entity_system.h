#include <map>
#include <vector>
#include "component.h"
typedef int FamilyId;
struct EntitySystem;

struct Entity
{
	static EntitySystem *entitySystem;
	Entity();
	template<typename Type> Type *getAs();
	std::map<FamilyId, Component*> mComponents;
};

EntitySystem *Entity::entitySystem = 0;

struct EntitySystem
{
		EntitySystem()
		{
			Entity::entitySystem = this;
		}
		template<typename T> T *getComponent(Entity *e)
		{
			return (T*)e->mComponents[T::familyId];
		}
		template<typename T> void getEntities(std::vector<Entity*> &result)
		{
			auto iterPair = mComponentStore.equal_range(T::familyId);
			for (auto iter = iterPair.first; iter != iterPair.second; ++iter)
			{
				result.push_back(iter->second);
			}
		}
		template<typename T> void addComponent(Entity *e, T* comp)
		{
			mComponentStore.insert(std::pair<FamilyId, Entity*>(T::familyId, e));
			e->mComponents.insert(std::pair<FamilyId, Component*>(T::familyId, comp));
		}
	protected:
		std::multimap<FamilyId, Entity*> mComponentStore;
};

inline Entity::Entity()
{
}

template<typename Type> Type *Entity::getAs()
{
	return entitySystem->getComponent<Type>(this);
}