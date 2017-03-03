#include "Wall.h"
#include "TileMap.h"
#include "Constants.h"
#include <SFML/Window/Keyboard.hpp>

Wall::Wall(TileMap* tileMap, int quadVertices, int arrayPos) :
	CollidableEntityDefault(EntityType::WALL),
	mTileMap(tileMap), mArrayPos(arrayPos) {
	mCat = CollideCategory::WALL;
	mQuadVertices = quadVertices;
}

Wall::~Wall() {
	mTileMap->removeVertices(mQuadVertices);
}

int Wall::getVertex() const {
	return mQuadVertices;
}

void Wall::setVertex(int num) {
	mQuadVertices = num;
}

void Wall::setCollisionCat(CollideCategory cat) {
	mCat = cat;
}

void Wall::tick(const sf::Time& deltaTime) {

}

void Wall::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		mGarbage = true;
		for (int i = 0; i < 4; i++) {

		}
	}
}
