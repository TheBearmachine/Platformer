#include "Inventory.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include "InventorySlot.h"
#include "Item.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

static const char* TEXTURE_FRAME = "Resources/Images/InventoryFrame.png";
static const int ITEM_RENDER_LAYER = 110;

Inventory::Inventory(EventManager* eventManager, EntityManager* entityManager) :
	mEventManager(eventManager),
	mEntityManager(entityManager),
	mInventorySlots(), mActive(false), mGarbage(false),
	mFrameNrY(0), mFrameNrX(0) {
	sf::Color col = { sf::Uint8(155), sf::Uint8(155), sf::Uint8(255), sf::Uint8(200) };
	mBackground.setFillColor(col);
	mBackground.setOutlineThickness(4.0f);
	setPosition(150.0f, 50.0f);
	mEventManager->registerObserver(this, sf::Event::EventType::KeyPressed);
}

Inventory::~Inventory() {
	mEventManager->unregisterObserver(this, sf::Event::EventType::KeyPressed);

	mInventorySlots.clear();
}

void Inventory::setupInventory(int width, int height) {
	mFrameNrX = width;
	mFrameNrY = height;
	sf::Texture* tex = &ResourceManager::getInstance().getTexture(TEXTURE_FRAME);
	sf::Vector2f vec = { (float)tex->getSize().x, (float)tex->getSize().y };
	sf::Color col = { sf::Uint8(255), sf::Uint8(255), sf::Uint8(255), sf::Uint8(200) };

	for (int i = 0; i < mFrameNrY; i++) {
		for (int j = 0; j < mFrameNrX; j++) {
			sf::Vector2f vec2((float)tex->getSize().x*(float)j,
				(float)tex->getSize().y*(float)i);
			InventorySlot slot = InventorySlot();
			slot.setTexture(TEXTURE_FRAME);
			slot.setPosition(vec2 + getPosition());
			mInventorySlots.push_back(slot);
		}
	}
	mWidth = vec.x*(float)width;
	mHeight = vec.y*(float)height;
	sf::Vector2f vec2 = { mWidth, mHeight };
	mBackground.setSize(vec2);
}

int Inventory::getFrameNrX() const {
	return mFrameNrX;
}

int Inventory::getFrameNrY() const {
	return mFrameNrY;
}

float Inventory::getWidth() const {
	return mWidth;
}

float Inventory::getHeight() const {
	return mHeight;
}

bool Inventory::getActive() const {
	return mActive;
}

// Iterate through the entire inventory to try and find an empty space for the item
void Inventory::addItem(Item* item, size_t startpoint) {
	for (size_t i = startpoint; i < mInventorySlots.size(); i++) {
		if (item == nullptr) return;

		item = addItemToSlot(item, i);
	}
	// There was no room for the item, throw it out into the world or some shit
	// TODO
}

// Function for adding a item to a specific slot in the inventory
Item* Inventory::addItemToSlot(Item* item, int slot) {
	// If slot is out of bounds or there is no item then return immediately
	if (slot > mFrameNrX * mFrameNrY || item == nullptr) return item;

	Item* invItem = mInventorySlots[slot].getContent();
	int newSlot = getNewSlot(slot, item->getItemInfo()->width, item->getItemInfo()->height);
	std::vector<int> indices(getArea(item, newSlot));
	bool empty = true;
	indices = getArea(item, newSlot);

	// If there is an item in any of the slots, return
	for (size_t i = 0; i < indices.size(); i++) {
		Item* itemToCheck = mInventorySlots[indices.at(i)].getContent();
		if (itemToCheck != nullptr) {
			return item;
		}
	}
	// If there indeed was an empty space then add the item there
	if (empty) {
		item->anchorToEntity(&mInventorySlots[newSlot]);
		item->setRenderLayer(NULL);
		for (size_t i = 0; i < indices.size(); i++)
			mInventorySlots[indices[i]].setContent(item);

		if (item->getItemInfo()->maxStack < item->getStackSize()) {
			int newSize = item->getStackSize() - item->getItemInfo()->maxStack;
			Item* newItem = new Item(item->getItemInfo()->ID, newSize);
			item->setMaxStack();
			mEntityManager->addEntity(newItem);
			newItem->setRenderLayer(NULL);
			return newItem;
		}
		return nullptr;
	}
	// If the inventory item has the same ID and is at less than
	// max stacks then merge the items
	else if (invItem->getItemInfo()->ID == item->getItemInfo()->ID &&
			 invItem->getItemInfo()->maxStack < invItem->getStackSize()) {
		// If there are more stacks in total than the max stack, create a new
		// instance
		if (invItem->getStackSize() + item->getStackSize() > invItem->getItemInfo()->maxStack) {
			int newSize = invItem->getStackSize() + item->getStackSize() - invItem->getItemInfo()->maxStack;
			invItem->setMaxStack();
			Item* newItem = new Item(item->getItemInfo()->ID, newSize);
			newItem->setRenderLayer(NULL);
			mEntityManager->addEntity(newItem);
			return item;
		}

		invItem->addToStack(item->getStackSize());
		item->garbage();
		return nullptr;
	}
	return item;
}

// Takes whatever item from the requested slot if any and hand over responsibility
Item* Inventory::takeItemFromSlot(int slot, Entity* anchor) {

	Item* invItem = mInventorySlots[slot].getContent();
	if (invItem == nullptr || anchor == nullptr) return nullptr;

	int newSlot = getNewSlot(invItem->getInventorySlot(), invItem->getItemInfo()->width, invItem->getItemInfo()->height);

	invItem->anchorToEntity(anchor);
	std::vector<int> indices = getArea(invItem, newSlot);
	for (size_t i = 0; i < indices.size(); i++) {
		mInventorySlots[indices[i]].setContent(nullptr);
	}
	return invItem;
}

Item* Inventory::swapItems(Item* item, int slot, Entity* anchor) {
	if (slot > mFrameNrX * mFrameNrY || item == nullptr) return item;

	Item* invItem = nullptr;
	int newSlot = getNewSlot(slot, item->getItemInfo()->width, item->getItemInfo()->height);
	std::vector<int> indices(getArea(item, newSlot));
	bool empty = true;
	indices = getArea(item, newSlot);
	Item* itemToCheck = mInventorySlots[slot].getContent();

	// Check to see if the requested slots have only a single item in them, or are empty
	for (size_t i = 0; i < indices.size(); i++) {
		invItem = mInventorySlots[indices[i]].getContent();
		if (invItem != nullptr || itemToCheck != invItem) {
			return item;
		}
		if (itemToCheck == nullptr)
			itemToCheck = mInventorySlots[indices[i]].getContent();
	}

	// If all slots were empty, no need to swap
	if (itemToCheck == nullptr) {

	}
	else {

	}

	std::vector<int> indices2();

	return nullptr;
}

InventorySlot* Inventory::getInventorySlot(int index) {
	return &mInventorySlots[index];
}

void Inventory::tick(const sf::Time& deltaTime) {
	if (!mActive) return;
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mActive) return;
	sf::RenderStates states1(states);
	states1.transform *= getTransform();
	target.draw(mBackground, states1);

	for (auto i : mInventorySlots) {
		target.draw(i, states);
	}
}

bool Inventory::garbage() const {
	return mGarbage;
}

void Inventory::kill() {
	mGarbage = true;
}

void Inventory::observe(const sf::Event& _event) {
	switch (_event.type) {
	case sf::Event::EventType::KeyPressed:
		if (_event.key.code == sf::Keyboard::I) {
			mActive = !mActive;
			for (size_t i = 0; i < mInventorySlots.size(); i++) {
				if (mInventorySlots[i].getContent() != nullptr)
					if (mActive)
						mInventorySlots[i].getContent()->setDrawMe(true);
					else
						mInventorySlots[i].getContent()->setDrawMe(false);
			}
		}
	default:
		break;
	}
}

// Returns a vector with all the indices of slots in the inventory a item occupies
std::vector<int> Inventory::getArea(Item* item, int slot) {
	int width = item->getItemInfo()->width,
		height = item->getItemInfo()->height;
	std::vector<int> indices;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			indices.push_back(slot + i + j*mFrameNrX);
		}
	}
	return indices;
}

// Corrects a slot to be within the the inventorys bounds based on the items size
int Inventory::getNewSlot(int slot, int width, int height) {
	if (slot % mFrameNrX + width > mFrameNrX) {
		int distanceFromEdge = mFrameNrX - (slot % mFrameNrX);
		slot = slot - (width - distanceFromEdge);
	}
	if (slot / mFrameNrX + height > mFrameNrY) {
		int distanceFromEdge = mFrameNrY - (slot / mFrameNrX);
		slot = slot - (width - distanceFromEdge)*mFrameNrX;
	}
	return slot;
}