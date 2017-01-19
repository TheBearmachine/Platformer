#include "Inventory.h"
#include "EventManager.h"
#include "ResourceManager.h"
#include "InventorySlot.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

static const char* TEXTURE_FRAME = "Images/InventoryFrame.png";

Inventory::Inventory(EventManager* eventManager) :
	mEventManager(eventManager),
	mInventorySlots(), mActive(false), mGarbage(false),
	mFramesHeight(0), mFramesWidth(0)
{
	sf::Color col = { sf::Uint8(155), sf::Uint8(155), sf::Uint8(255), sf::Uint8(200) };
	mBackground.setFillColor(col);
	mBackground.setOutlineThickness(4.0f);
	setPosition(150.0f, 50.0f);
	mEventManager->registerObserver(this, sf::Event::EventType::KeyPressed);
}

Inventory::~Inventory()
{
	mEventManager->unregisterObserver(this, sf::Event::EventType::KeyPressed);
	/*while (!mInventorySlots.empty()) {
		delete mInventorySlots.back();
		mInventorySlots.pop_back();
	}*/
	mInventorySlots.clear();
}

void Inventory::setupInventory(int width, int height) {
	mFramesWidth = width;
	mFramesHeight = height;
	sf::Texture* tex = &ResourceManager::getInstance().getTexture(TEXTURE_FRAME);
	sf::Vector2f vec = {(float)tex->getSize().x, (float)tex->getSize().y };
	sf::Color col = { sf::Uint8(255), sf::Uint8(255), sf::Uint8(255), sf::Uint8(200) };
	for (int i = 0; i < mFramesHeight; i++) {
		for (int j = 0; j < mFramesWidth; j++) {
			sf::Vector2f vec2((float)tex->getSize().x*(float)i,
				(float)tex->getSize().y*(float)j);
			InventorySlot slot = InventorySlot();
			slot.setTexture(TEXTURE_FRAME);
			slot.setPosition(vec2);
			mInventorySlots.push_back(slot);
			//mInventoryIcons.push_back(sprite);
		}
	}
	sf::Vector2f vec2 = { vec.x*(float)width, vec.y*(float)height};
	mBackground.setSize(vec2);
}

void Inventory::tick(const sf::Time& deltaTime) {
	if (!mActive) return;
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!mActive) return;
	states.transform *= getTransform();
	target.draw(mBackground, states);

	for (auto i : mInventorySlots) {
		/*sf::RenderStates states2(states);
		states2.transform *= i.getTransform();*/
		target.draw(i, states);
	}
}

bool Inventory::garbage() const {
	return false;
}

void Inventory::kill() {
	mGarbage = true;
}

void Inventory::observe(const sf::Event & _event) {
	switch (_event.type) {
	case sf::Event::EventType::KeyPressed:
		if (_event.key.code == sf::Keyboard::I) {
			mActive = !mActive;
		}
	default:
		break;
	}
}
