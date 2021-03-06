#pragma once

#include "Entity.h"
#include "EventObserver.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <vector>

class EventManager;
class EntityManager;
class InventorySlot;
class Item;

class Inventory : public Entity, public EventObserver {
public:
	Inventory(EventManager* eventManager, EntityManager* entityManager);
	~Inventory();

	void setupInventory(int width, int height);
	int getFrameNrX() const;
	int getFrameNrY() const;
	float getWidth() const;
	float getHeight() const;
	bool getActive() const;
	void addItem(Item* item, size_t startpoint = 0);
	Item* addItemToSlot(Item* item, int slot);
	Item* takeItemFromSlot(int slot, Entity* anchor);
	Item* swapItems(Item* item, int slot, Entity* anchor);
	InventorySlot* getInventorySlot(int index);

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;
	virtual void observe(const sf::Event & _event) override;

private:
	std::vector<int> getArea(Item* item, int slot);
	int getNewSlot(int slot, int width, int height);

	std::vector<InventorySlot> mInventorySlots;
	sf::RectangleShape mBackground;
	EventManager* mEventManager;
	EntityManager* mEntityManager;

	int mRenderLayer;
	int mFrameNrX, mFrameNrY;
	float mWidth, mHeight;
	bool mGarbage;
	bool mActive;
};