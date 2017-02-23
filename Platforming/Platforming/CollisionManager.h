#ifndef _INCLUDED_COLLISIONMANAGER_H_
#define _INCLUDED_COLLISIONMANAGER_H_

#include "CollidableEntity.h"
#include <vector>

namespace sf {
	class RenderWindow;
}
class Item;
class TileMap;

class CollisionManager {
public:
	typedef std::vector<CollidableEntity*> CollidableVector;
	CollisionManager(sf::RenderWindow* window);
	~CollisionManager();

	void addDynamicCollidable(CollidableEntity* collidable);
	void addItemCollidable(CollidableEntity* collidable);
	void addTileMap(TileMap* tileMap);
	const CollidableVector& getDynamicCollidables() const;
	//TileMap& getTileMap();
	void detectCollisions();
	void removeDeadCollidables();
	void clearVector();
	void narrowCollision(std::pair<CollidableEntity*, CollidableEntity*>& colliding);

private:
	sf::RenderWindow* mWindow;
	CollidableVector mDynamicCollidables;
	CollidableVector mItems;
	TileMap* mTileMap;
};
#endif // !_INCLUDED_COLLISIONMANAGER_H_
