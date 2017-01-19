#ifndef _INCLUDED_TILEMAP_H_
#define _INCLUDED_TILEMAP_H_

#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "Entity.h"

class CollisionManager;
class CollidableEntity;
class Wall;

class TileMap : public Entity {
public:
	TileMap(CollisionManager* collisionManager);
	~TileMap();

	void removeVertices(int vertex);
	bool load(const char* tileset, sf::Vector2u tileSize,int* tiles, unsigned width, unsigned height);
	CollidableEntity** getWallHashTable();
	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;

private:
	CollisionManager* mCollisionManager;
	//static const int WIDTH, HEIGHT;
	sf::VertexArray mVertices;
	sf::Texture* mTileset;
	sf::Vector2u mTileSize;
	int* mTiles;
	CollidableEntity* mWallHashTable[512*128];
};

#endif // !_INCLUDED_TILEMAP_H_
