#ifndef _INCLUDED_COLLISIONMANAGER_H_
#define _INCLUDED_COLLISIONMANAGER_H_

#include "CollidableEntity.h"
#include <vector>

namespace sf {
	class RenderWindow;
}

class CollisionManager {
public:
	typedef std::vector<CollidableEntity*> CollidableVector;
	CollisionManager(sf::RenderWindow* window);
	~CollisionManager();

	void addDynamicCollidable(CollidableEntity* collidable);
	void addStaticCollidable(CollidableEntity* collidable);
	const CollidableVector& getDynamicCollidables() const;
	const CollidableVector& getStaticCollidables() const;
	void detectCollisions();
	void removeDeadCollidables();
	void clearVector();
	void narrowCollision(std::pair<CollidableEntity*, CollidableEntity*>& colliding);

private:
	sf::RenderWindow* mWindow;
	CollidableVector mDynamicCollidables;
	CollidableVector mStaticCollidables;
};
#endif // !_INCLUDED_COLLISIONMANAGER_H_
