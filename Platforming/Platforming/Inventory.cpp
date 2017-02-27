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

void Inventory::addItem(Item* item) {
	if (item == nullptr) return;

	for (size_t i = 0; i < mInventorySlots.size(); i++) {
		Item* invItem = mInventorySlots[i].getContent();

		// If there is no item add the new item and check if the
		// item exceeds allowed inventory stack limit
		if (invItem == nullptr) {
			item->setRenderLayer(110);
			item->anchorToEntity(&mInventorySlots[i]);
			mInventorySlots[i].setContent(item);
			item->anchorToEntity(&mInventorySlots[i]);

			if (item->getItemInfo()->maxStack < item->getStackSize()) {
				int newSize = item->getStackSize() - item->getItemInfo()->maxStack;
				Item* newItem = new Item(item->getItemInfo()->ID, newSize);
				item->setMaxStack();
				mEntityManager->addEntity(newItem);
				newItem->setRenderLayer(item->getRenderLayer());
				addItem(newItem);
			}
			return;
		}

		// If the item has the same ID and is at less than max stacks
		// then merge the items
		else if (invItem->getItemInfo()->ID == item->getItemInfo()->ID &&
				 invItem->getItemInfo()->maxStack < item->getStackSize()) {
			// If there are more stacks in total than the max stack, create a new
			// instance and
			item->setRenderLayer(110);
			if (invItem->getStackSize() + item->getStackSize() > invItem->getItemInfo()->maxStack) {
				int newSize = invItem->getStackSize() + item->getStackSize() - invItem->getItemInfo()->maxStack;
				invItem->setMaxStack();
				Item* newItem = new Item(item->getItemInfo()->ID, newSize);
				newItem->setRenderLayer(item->getRenderLayer());
				mEntityManager->addEntity(newItem);
				addItem(newItem);
			}
			else {
				item->garbage();
			}
			return;
		}
	}
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
