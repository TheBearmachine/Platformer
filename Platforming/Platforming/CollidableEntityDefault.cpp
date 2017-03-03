#include "CollidableEntityDefault.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include "VectorMath.h"

CollidableEntityDefault::CollidableEntityDefault(EntityType entityType) :
	mGarbage(false), mSprite(sf::Vector2f(32, 32)),
	CollidableEntity(entityType) {
	mHitboxShape = new sf::RectangleShape(mSprite);
	// Remove later
	//mSprite.setOutlineThickness(5.0f);
	mSprite.setFillColor(sf::Color::Red);
	mSprite.setOutlineColor(sf::Color::Magenta);
	// Remove to here
	updateAxes();
}

CollidableEntityDefault::~CollidableEntityDefault() {
	delete mHitboxShape;
}

void CollidableEntityDefault::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool CollidableEntityDefault::garbage() const {
	return mGarbage;
}

void CollidableEntityDefault::kill() {
	mGarbage = true;
}

void CollidableEntityDefault::setSprite(sf::Vector2f size) {
	mSprite = sf::RectangleShape(size);
	delete mHitboxShape;
	mHitboxShape = new sf::RectangleShape(mSprite);
	updateAxes();
}

void CollidableEntityDefault::setSprite(float width, float height) {
	mSprite = sf::RectangleShape({ width, height });
	delete mHitboxShape;
	mHitboxShape = new sf::RectangleShape(mSprite);
	updateAxes();
}

CollidableEntity::CollideCategory CollidableEntityDefault::getCategory() const {
	return mCat;
}

std::vector<sf::Vector2f> CollidableEntityDefault::getAxes() const {
	return mAxes;
}

sf::FloatRect CollidableEntityDefault::getHitbox() const {
	//Might return fuckd up results
	return getTransform().transformRect(mSprite.getGlobalBounds());
}

sf::Shape* CollidableEntityDefault::getNarrowHitbox() const {
	if (mHitboxShape != nullptr) {
		mHitboxShape->setPosition(getPosition());
		mHitboxShape->setScale(getScale());
		mHitboxShape->setRotation(getRotation());
		return mHitboxShape;
	}
	return nullptr;
}

void CollidableEntityDefault::updateAxes() {
	mAxes.clear();
	std::size_t pointCount = mHitboxShape->getPointCount();
	for (std::size_t i = 0; i < pointCount; i++) {
		sf::Transform trans = mHitboxShape->getTransform();
		sf::Vector2f v1 = mHitboxShape->getPoint(i + 1 == pointCount ? 0 : i + 1);
		sf::Vector2f v2 = mHitboxShape->getPoint(i);
		v1 = trans.transformPoint(v1);
		v2 = trans.transformPoint(v2);
		mAxes.push_back(VectorMath::normalizeVector(VectorMath::getNormal(v2 - v1)));
	}
}
