#ifndef _INCLUDED_ENTITYMANAGER_H_
#define _INCLUDED_ENTITYMANAGER_H_

#include <vector>

class Entity;

namespace sf {
	class Time;
	class RenderWindow;
}

class EntityManager {
public:
	EntityManager();
	virtual ~EntityManager();

	// Higher layers are drawn later, and layer >= 100 will be drawn statically
	void addEntity(Entity* entity);

	void updateEntities(const sf::Time& deltaTime);
	void renderElements(sf::RenderWindow& window);
	void garbageCollection();

private:
	void internalCleanup();

	std::vector<Entity*> mEntities;
};

#endif // !_INCLUDED_ENTITYMANAGER_H_
