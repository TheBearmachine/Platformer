#ifndef _INCLUDED_WALL_H_
#define _INCLUDED_WALL_H_

#include "CollidableEntityDefault.h"
class TileMap;

class Wall : public CollidableEntityDefault{
public:
	Wall(TileMap* tileMap, int quadVertices, int arrayPos);
	~Wall();

	int getVertex() const;
	void setVertex(int num);
	void setCollisionCat(CollideCategory cat);
	virtual void tick(const sf::Time& deltaTime) override;
	virtual void collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) override;

private:
	TileMap* mTileMap;
	int mArrayPos;
	int mQuadVertices;
};

#endif // !_INCLUDED_WALL_H_
