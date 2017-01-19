#pragma once

#include "Entity.h"
#include "EventObserver.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class EventManager;
class InventorySlot;

class Inventory : public Entity, public EventObserver {
public:
	Inventory(EventManager* eventManager);
	~Inventory();

	void setupInventory(int width, int height);

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;
	virtual void observe(const sf::Event & _event) override;

private:
	std::vector<InventorySlot> mInventorySlots;
	sf::RectangleShape mBackground;
	EventManager* mEventManager;

	int mFramesWidth, mFramesHeight;
	bool mGarbage;
	bool mActive;
};