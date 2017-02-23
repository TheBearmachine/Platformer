#include "InventorySlot.h"
#include "ResourceManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

InventorySlot::InventorySlot() :
	mGarbage(false) {

}

InventorySlot::~InventorySlot() {

}

void InventorySlot::setTexture(const char* filename) {
	mBackground.setTexture(ResourceManager::getInstance().getTexture(filename));
}

Item* InventorySlot::getContent() {
	return mContent;
}

void InventorySlot::setContent(Item* item) {
	mContent = item;
}

void InventorySlot::tick(const sf::Time& deltaTime) {

}

void InventorySlot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mBackground, states);
}

bool InventorySlot::garbage() const {
	return mGarbage;
}

void InventorySlot::kill() {
	mGarbage = true;
}
