#include "CollisionManager.h"
#include "TileMap.h"
#include "Item.h"
#include "VectorMath.h"
#include "Constants.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <stack>

static const int COLLISION_RADIUS = 200;

CollisionManager::CollisionManager(sf::RenderWindow* window) :
	mWindow(window) {

}

CollisionManager::~CollisionManager() {
	clearVector();
}

void CollisionManager::addDynamicCollidable(CollidableEntity* collidable) {
	mDynamicCollidables.push_back(collidable);
}

void CollisionManager::addTileMap(TileMap* tileMap) {
	mTileMap = tileMap;
}

const CollisionManager::CollidableVector& CollisionManager::getDynamicCollidables() const {
	return mDynamicCollidables;
}

//TileMap& CollisionManager::getTileMap(){
//	return mTileMap;
//}

std::pair<float, float> getProjection(const sf::Shape& shape, sf::Vector2f& axis) {
	sf::Transform trans = shape.getTransform();

	float minVal = VectorMath::dotProduct(axis, VectorMath::projectVector(trans.transformPoint(shape.getPoint(0)), axis));
	float maxVal = minVal;

	for (std::size_t i = 1; i < shape.getPointCount(); i++) {
		float temp = VectorMath::dotProduct(axis, VectorMath::projectVector(trans.transformPoint(shape.getPoint(i)), axis));
		if (temp < minVal)
			minVal = temp;
		else if (temp > maxVal)
			maxVal = temp;
	}
	return std::make_pair(minVal, maxVal);
}

void CollisionManager::detectCollisions() {
	sf::FloatRect bounds(mWindow->mapPixelToCoords({ 0, 0 }),
		mWindow->getView().getSize());
	bounds.left -= 50.0f;
	bounds.top -= 50.0f;
	bounds.height += 100.0f;
	bounds.width += 100.0f;

	//Culls collision to only account for active against static so that
	//things like walls arent checked against eachother
	std::stack<std::pair<CollidableEntity*, CollidableEntity*>> colliding;

	CollidableVector staticCollidables;
	for (std::size_t i = 0; i < mDynamicCollidables.size(); i++) {
		CollidableEntity* collidable0 = mDynamicCollidables[i];
		for (std::size_t j = i + 1; j < mDynamicCollidables.size(); j++) {
			CollidableEntity *collidable1 = mDynamicCollidables[j];
			if (collidable0->getHitbox().intersects(collidable1->getHitbox()) && (collidable0->getCategory() != collidable1->getCategory())) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
		int position = (int)collidable0->getPosition().x / Constants::Block::Width +
			((int)collidable0->getPosition().y / Constants::Block::Height) * Constants::Map::Width;

		//Get a square grid of 5x5 around the position wrapped around the map size to avoid out of bounds
		for (int j = -2; j < 3; j++) {
			for (int k = -2; k < 3; k++) {
				int absPos = abs(position + j + k*Constants::Map::Width)
					% (Constants::Map::Height*Constants::Map::Width);
				CollidableEntity* entity = mTileMap->getWallHashTable()[absPos];
				if (entity != nullptr)
					staticCollidables.push_back(entity);
			}
		}

		// Check dynamic entities against items
		for (std::size_t j = 0; j < mItems.size(); j++) {
			CollidableEntity* collidable1 = mItems[j];
			Item* item;
			item = dynamic_cast<Item*>(collidable1);
			if (item->getAnchor() == nullptr &&
				(collidable0->getHitbox().intersects(collidable1->getHitbox()) &&
					(collidable0->getCategory() != collidable1->getCategory()))) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}

		// Check Dynamic entities against static entities
		for (std::size_t j = 0; j < staticCollidables.size(); j++) {
			CollidableEntity *collidable1 = staticCollidables[j];
			if (collidable0->getHitbox().intersects(collidable1->getHitbox()) && (collidable0->getCategory() != collidable1->getCategory())) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
		staticCollidables.clear();
	}
	while (!colliding.empty()) {
		narrowCollision(colliding.top());
		colliding.pop();
	}
}

void CollisionManager::removeDeadCollidables() {
	CollidableVector vec;
	for (auto c : mDynamicCollidables) {
		if (!c->garbage())
			vec.push_back(c);
	}
	mDynamicCollidables = vec;
	vec.clear();

}

void CollisionManager::clearVector() {
	mDynamicCollidables.clear();
}

void CollisionManager::narrowCollision(std::pair<CollidableEntity*, CollidableEntity*>& colliding) {
	bool isColliding = true;
	auto hitboxPair = std::make_pair(colliding.first->getNarrowHitbox(), colliding.second->getNarrowHitbox());
	sf::Vector2f smallest(0, 0);
	float overlap = 5000;

	std::vector<sf::Vector2f> axes1 = colliding.first->getAxes();
	std::vector<sf::Vector2f> axes2 = colliding.second->getAxes();
	std::size_t pointCount = axes1.size();

	for (std::size_t i = 0; i < pointCount; i++) {
		std::pair<float, float> shapeProj[2];
		shapeProj[0] = getProjection(*hitboxPair.first, axes1[i]);
		shapeProj[1] = getProjection(*hitboxPair.second, axes1[i]);
		if (shapeProj[0].first > shapeProj[1].second || shapeProj[1].first > shapeProj[0].second)
			return;
		else {
			if (shapeProj[0].first < shapeProj[1].second && shapeProj[0].first > shapeProj[1].first) {
				float o = shapeProj[1].second - shapeProj[0].first;
				if (o < overlap) {
					overlap = o;
					smallest = axes1[i];
				}
			}
			else if (shapeProj[0].second > shapeProj[1].first && shapeProj[0].second < shapeProj[1].second) {
				float o = shapeProj[0].second - shapeProj[1].first;
				if (o < overlap) {
					overlap = o;
					smallest = -axes1[i];
				}
			}
		}
	}
	pointCount = axes2.size();
	for (std::size_t i = 0; i < pointCount; i++) {
		std::pair<float, float> shapeProj[2];
		shapeProj[0] = getProjection(*hitboxPair.first, axes2[i]);
		shapeProj[1] = getProjection(*hitboxPair.second, axes2[i]);
		if (shapeProj[0].first >= shapeProj[1].second || shapeProj[1].first >= shapeProj[0].second)
			return;

		else {
			if (shapeProj[0].first <= shapeProj[1].second && shapeProj[0].first >= shapeProj[1].first) {
				float o = shapeProj[1].second - shapeProj[0].first;
				if (o < overlap) {
					overlap = o;
					smallest = axes2[i];
				}
			}
			else if (shapeProj[0].second > shapeProj[1].first && shapeProj[0].second < shapeProj[1].second) {
				float o = shapeProj[0].second - shapeProj[1].first;
				if (o < overlap) {
					overlap = o;
					smallest = -axes2[i];
				}
			}
		}
	}
	colliding.first->collide(colliding.second, smallest * overlap);
	colliding.second->collide(colliding.first, -smallest * overlap);
}