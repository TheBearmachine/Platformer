#pragma once

#include "EventObserver.h"

class EventManager;
class MouseCursor;
class Inventory;
class Item;

class ItemManager : public EventObserver {
public:
	ItemManager(EventManager* eventManager);
	~ItemManager();
	bool initialize(MouseCursor* cursor, Inventory* inventory);

	virtual void observe(const sf::Event& _event) override;

private:
	MouseCursor* mCursor;
	Inventory* mInventory;
	EventManager* mEventManager;
	Item* mCursorItem;

	// Inherited via EventObserver
};