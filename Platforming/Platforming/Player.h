#ifndef _INCLUDED_PLAYER_H_
#define _INCLUDED_PLAYER_H_

#include "CollidableEntityDefault.h"
#include <vector>

class MouseCursor;
class EntityManager;
namespace sf {
	class RenderWindow;
}

class Player : public CollidableEntityDefault{
public:
	Player(sf::RenderWindow* window, EntityManager* entityManager);
	~Player();

	void setCursor(MouseCursor* cursor);
	virtual void tick(const sf::Time & deltaTime) override;
	virtual void collide(CollidableEntity * collidable, const sf::Vector2f & moveAway) override;

private:
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	MouseCursor* mCursor;
	
	sf::Vector2f mVelocity;
	sf::Vector2f mCollisionVector;
	std::vector<CollidableEntity*> mWallCollides;
	int mCollisions;
	float mFriction;
	bool mCanJump;
};

#endif // !_INCLUDED_PLAYER_H_
