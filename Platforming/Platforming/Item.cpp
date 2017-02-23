#include "Item.h"
#include "Constants.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include<SFML/System/Time.hpp>

Item::Item(int itemID) :
	Item(itemID, 1) {
	setShitUp();
}

Item::Item(int itemID, int stackSize) :
	CollidableEntityDefault(),
	mVelocity(0.0f, 0.0f),
	mStackSize(stackSize),
	mItemInfo(ItemDatabase::getInstance().getItemInfo(itemID)) {
	setShitUp();
}

void Item::setShitUp() {
	mSprite.setTexture(ResourceManager::getInstance().getTexture(mItemInfo->imageFilename));
	mSprite.setTextureRect(sf::IntRect(0, 0, 32 * mItemInfo->width, 32 * mItemInfo->height));
	mStackText.setCharacterSize(8);
	mStackText.setString(sf::String((uint32_t)mStackSize));
}

Item::~Item() {

}

void Item::tick(const sf::Time& deltaTime) {
	if (mAnchor != nullptr) return;
	mVelocity.y += Constants::Physics::Gravity*deltaTime.asSeconds();
	mVelocity.y = std::min(mVelocity.y, Constants::Physics::TerminalVelocity.y);
	mVelocity.y = std::max(mVelocity.y, -Constants::Physics::TerminalVelocity.y);

	move(mVelocity);
}

void Item::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mDrawMe) return;
	if (mAnchor != nullptr)
		states.transform *= mAnchor->getTransform();

	else
		states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mStackText, states);
}

void Item::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {
	if (collidable->getCategory() == CollideCategory::WALL) {
		mVelocity.x = 0.0f;
		move(moveAway);
	}
	else if (collidable->getCategory() == CollideCategory::FLOOR) {
		mVelocity.y = 0.0f;
		move(moveAway);
	}
}

ItemDatabase::ItemStruct* Item::getItemInfo() {
	return mItemInfo;
}

void Item::anchorToEntity(Entity* entity) {
	mAnchor = entity;
}

Entity* Item::getAnchor() const {
	return mAnchor;
}

int Item::getStackSize() const {
	return mStackSize;
}

void Item::addToStack(int size) {
	mStackSize = std::max(mStackSize += size, 1);
	mStackText.setString(sf::String((uint32_t)mStackSize));
}

void Item::setMaxStack() {
	mStackSize = mItemInfo->maxStack;
	mStackText.setString(sf::String((uint32_t)mStackSize));
}

void Item::setDrawMe(bool toDraw) {
	mDrawMe = toDraw;
}
