#include "TileMap.h"
#include "CollisionManager.h"
#include "Wall.h"
#include <SFML/Graphics/RenderWindow.hpp>

TileMap::TileMap(CollisionManager* collisionManager) :
	mCollisionManager(collisionManager) {

}

TileMap::~TileMap() {

}

void TileMap::removeVertices(int vertex) {
	size_t count = mVertices.getVertexCount();
	for (size_t i = 0; i < 4; i++) {
		mVertices[vertex + i] = mVertices[count - 4 + i];
	}
	for (auto t : mTileCollisions) {
		int vert = t->getVertex();
		if (vert == count - 4) {
			t->setVertex(vertex);
			break;
		}
	}
	mVertices.resize(count - 4);
}

bool TileMap::load(const char* tileset, sf::Vector2u tileSize, int* tiles, unsigned width, unsigned height) {
	if (tileset != nullptr)
		if (!mTileset.loadFromFile(tileset)) {
			printf("Failed to load file %s", tileset);
			return false;
		}

	mTileSize = tileSize;
	mTiles = tiles;
	mVertices.setPrimitiveType(sf::Quads);
	mVertices.resize(width * height * 4);

	for (unsigned i = 0; i < width; i++) {
		for (unsigned j = 0; j < height; j++) {
			int tileNumber = mTiles[i + j * width];
			if (tileNumber == -1)
				continue;

			int tu = tileNumber % (mTileset.getSize().x / mTileSize.x);
			int tv = tileNumber / (mTileset.getSize().x / mTileSize.x);
			int pos = (i + j * width) * 4;
			sf::Vertex* quad = &mVertices[pos];
			quad[0].position = sf::Vector2f((float)(i * mTileSize.x), (float)(j * mTileSize.y));
			quad[1].position = sf::Vector2f((float)((i + 1) * mTileSize.x), (float)(j * mTileSize.y));
			quad[2].position = sf::Vector2f((float)((i + 1) * mTileSize.x), (float)((j + 1) * mTileSize.y));
			quad[3].position = sf::Vector2f((float)(i * mTileSize.x), (float)((j + 1) * mTileSize.y));

			quad[0].texCoords = sf::Vector2f((float)(tu * mTileSize.x), (float)(tv * mTileSize.y));
			quad[1].texCoords = sf::Vector2f((float)((tu + 1) * mTileSize.x), (float)(tv * mTileSize.y));
			quad[2].texCoords = sf::Vector2f((float)((tu + 1) * mTileSize.x), (float)((tv + 1) * mTileSize.y));
			quad[3].texCoords = sf::Vector2f((float)(tu * mTileSize.x), (float)((tv + 1) * mTileSize.y));

			Wall* wall = new Wall(this, pos);
			wall->setSprite((float)tileSize.x, (float)tileSize.y);
			wall->setPosition(quad[0].position);
			mTileCollisions.push_back(wall);
			mCollisionManager->addStaticCollidable(wall);
		}
	}
	return true;
}

void TileMap::tick(const sf::Time & deltaTime) {
	std::vector<Wall*> temp;
	for (auto t : mTileCollisions) {
		if (t->garbage())
			delete t;
		else
			temp.push_back(t);
	}
	mTileCollisions = temp;

}

void TileMap::draw(sf::RenderTarget & target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &mTileset;
	target.draw(mVertices, states);
}

bool TileMap::garbage() const {
	return false;
}

void TileMap::kill() {

}
