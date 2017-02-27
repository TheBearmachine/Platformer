#include "ItemManager.h"
#include "EventManager.h"
#include "MouseCursor.h"
#include "Item.h"
#include "Inventory.h"
#include "InventorySlot.h"
#include "Constants.h"

ItemManager::ItemManager(EventManager* eventManager) :
	mEventManager(eventManager) {
	mEventManager->registerObserver(this, sf::Event::MouseButtonPressed);
}

ItemManager::~ItemManager() {
	mEventManager->unregisterObserver(this, sf::Event::MouseButtonPressed);
}

bool ItemManager::initialize(MouseCursor* cursor, Inventory* inventory) {
	if ((mCursor = cursor) != nullptr && (mInventory = inventory) != nullptr)
		return false;

	return true;
}

void ItemManager::observe(const sf::Event& _event) {
	switch (_event.type) {
	case sf::Event::MouseButtonPressed: {
		// If inventory is closed, do nothing
		if (!mInventory->getActive())
			return;

		float x = mCursor->getPosition().x;
		float xmin = mInventory->getPosition().x;
		float xmax = mInventory->getWidth() + xmin;
		float y = mCursor->getPosition().y;
		float ymin = mInventory->getPosition().y;
		float ymax = mInventory->getHeight() + ymin;

		// Check if the cursor is within the inventory's boundaries
		if (x > xmin && x < xmax && y > ymin && y < ymax) {
			// Then convert coordinates into inventory slot id
			float slotWidth = mInventory->getWidth() / (float)mInventory->getFrameNrX();
			float slotHeight = mInventory->getHeight() / (float)mInventory->getFrameNrY();
			x = x - xmin;
			y = y - ymin;
			int slotX = (int)(x / slotWidth);
			int slotY = (int)(y / slotHeight);

			int slot = slotX + mInventory->getFrameNrX()*slotY;
			InventorySlot* invSlot = mInventory->getInventorySlot(slot);
			Item* item;
			if (mCursorItem == nullptr) {
				if ((item = invSlot->getContent()) != nullptr) {
					mCursorItem = item;
					item->anchorToEntity(mCursor);
					invSlot->setContent(nullptr);
				}
			}
			else {
				// If mouse is holding an item, drop it
				if (invSlot->getContent() == nullptr) {
					mCursorItem->anchorToEntity(invSlot);
					invSlot->setContent(mCursorItem);
					mCursorItem = nullptr;
				}
				// 
				else {
					Item* temp;
					temp = mCursorItem;
					mCursorItem->anchorToEntity(invSlot);
					mCursorItem = invSlot->getContent();
					invSlot->setContent(temp);
					mCursorItem->anchorToEntity(mCursor);
				}
			}
		}
		break;
	}
	default: {

		break;
	}
	}
}

