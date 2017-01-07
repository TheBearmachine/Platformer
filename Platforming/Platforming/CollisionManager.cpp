#include "CollisionManager.h"
#include "VectorMath.h"
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <stack>

static const int COLLISION_RADIUS = 200;

CollisionManager::CollisionManager(sf::RenderWindow* window):
mWindow(window){

}

CollisionManager::~CollisionManager() {
	clearVector();
}

void CollisionManager::addDynamicCollidable(CollidableEntity* collidable) {
	mDynamicCollidables.push_back(collidable);
}

void CollisionManager::addStaticCollidable(CollidableEntity* collidable) {
	mStaticCollidables.push_back(collidable);
}

const CollisionManager::CollidableVector& CollisionManager::getDynamicCollidables() const {
	return mDynamicCollidables;
}

const CollisionManager::CollidableVector& CollisionManager::getStaticCollidables() const {
	return mStaticCollidables;
}

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
	sf::FloatRect bounds(mWindow->mapPixelToCoords({0, 0}),
		mWindow->getView().getSize());
	bounds.left -= 50.0f;
	bounds.top -= 50.0f;
	bounds.height += 100.0f;
	bounds.width += 100.0f;


	//Culls collision to only account for active against static so that
	//things like walls arent checked against eachother
	std::stack<std::pair<CollidableEntity*, CollidableEntity*>> colliding;

	CollidableVector staticCollidables;
	for (std::size_t i = 0; i < mStaticCollidables.size(); i++) {
		if (mStaticCollidables[i]->getCategory() == CollidableEntity::CollideCategory::WALL &&
			bounds.contains(mStaticCollidables[i]->getPosition()))
			staticCollidables.push_back(mStaticCollidables[i]);

	}
	for (std::size_t i = 0; i < mDynamicCollidables.size(); i++) {
		CollidableEntity* collidable0 = mDynamicCollidables[i];
		for (std::size_t j = i + 1; j < mDynamicCollidables.size(); j++) {
			CollidableEntity *collidable1 = mDynamicCollidables[j];
			if (collidable0->getHitbox().intersects(collidable1->getHitbox()) && (collidable0->getCategory() != collidable1->getCategory())) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
		for (std::size_t j = 0; j < staticCollidables.size(); j++) {
			CollidableEntity *collidable1 = staticCollidables[j];
			if (collidable0->getHitbox().intersects(collidable1->getHitbox()) && (collidable0->getCategory() != collidable1->getCategory())) {
				colliding.push(std::make_pair(collidable0, collidable1));
			}
		}
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

	for (auto c : mStaticCollidables) {
		if (!c->garbage())
			vec.push_back(c);
	}
	mStaticCollidables = vec;
}

void CollisionManager::clearVector() {
	mDynamicCollidables.clear();
	mStaticCollidables.clear();
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