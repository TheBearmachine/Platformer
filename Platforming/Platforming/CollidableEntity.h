#ifndef _INCLUDED_COLLIDABLEENTITY_H_
#define _INCLUDED_COLLIDABLEENTITY_H_

#include "Entity.h"
#include <SFML/Graphics/Shape.hpp>
#include <vector>

class CollidableEntity : public Entity{
public:
	enum CollideCategory {
		PLAYER,
		PLAYER_PROJECTILE,
		CURSOR,
		ENEMY,
		ENEMY_PROJECTILE,
		WALL,
		FLOOR,
		IGNORE
	};

	CollidableEntity();
	virtual ~CollidableEntity();

	virtual CollideCategory getCategory() const = 0;
	virtual std::vector<sf::Vector2f> getAxes() const = 0;
	virtual void collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) = 0;
	virtual sf::FloatRect getHitbox() const = 0;
	virtual sf::Shape* getNarrowHitbox() const = 0;
};

#endif // !_INCLUDED_COLLIDABLEENTITY_H_
