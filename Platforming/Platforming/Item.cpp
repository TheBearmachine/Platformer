#include "Item.h"
#include "Constants.h"
#include "ResourceManager.h"
#include "InventorySlot.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include<SFML/System/Time.hpp>

Item::Item(int itemID) :
	Item(itemID, 1) {

}

Item::Item(int itemID, int stackSize) :
	CollidableEntityDefault(EntityType::ITEM),
	mVelocity(0.0f, 0.0f),
	mStackSize(stackSize),
	minventorySlot(-1),
	mStackText("", ResourceManager::getInstance().getFont(Constants::Files::Default_Font)),
	mItemInfo(ItemDatabase::getInstance().getItemInfo(itemID)) {
	setShitUp();
}

void Item::setShitUp() {
	mSprite.setTexture(ResourceManager::getInstance().getTexture(mItemInfo->imageFilename));
	mSprite.setTextureRect(sf::IntRect(0, 0, 32 * mItemInfo->width, 32 * mItemInfo->height));
	mStackText.setString(std::to_string(mStackSize));
	mStackText.setOutlineThickness(1.0f);
	mStackText.setOutlineColor(sf::Color::Black);
	mStackText.setFillColor(sf::Color::White);
	mStackText.setCharacterSize(12);
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
	if (mItemInfo->maxStack > 1)
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

void Item::setRenderLayer(int layer) {
	mRenderLayer = layer;
}

int Item::getRenderLayer() const {
	return (mAnchor != nullptr ? std::min(mAnchor->getRenderLayer() + 1, 200) : mRenderLayer);
}

ItemDatabase::ItemStruct* Item::getItemInfo() {
	return mItemInfo;
}

void Item::anchorToEntity(Entity* entity) {
	mAnchor = entity;
	if (entity->getEntityType() == EntityType::INVENTORYSLOT) {
		InventorySlot* invSlot = dynamic_cast<InventorySlot*>(entity);
		minventorySlot = invSlot->getIndex();
	}
	else
		minventorySlot = -1;
}

Entity* Item::getAnchor() const {
	return mAnchor;
}

int Item::getStackSize() const {
	return mStackSize;
}

void Item::addToStack(int size) {
	mStackSize = std::max(mStackSize += size, 1);
	mStackText.setString(std::to_string(mStackSize));
}

void Item::setMaxStack() {
	mStackSize = mItemInfo->maxStack;
	mStackText.setString(std::to_string(mStackSize));
}

void Item::setDrawMe(bool toDraw) {
	mDrawMe = toDraw;
}

int Item::getInventorySlot() const {
	return minventorySlot;
}

void Item::setInventorySlot(int slot) {
	minventorySlot = slot;
}
