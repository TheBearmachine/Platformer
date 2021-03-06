#pragma once

#include "CollidableEntityDefault.h"
#include "ItemDatabase.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class InventorySlot;

class Item : public CollidableEntityDefault {
public:
	Item(int itemID);
	Item(int itemID, int stackSize);
	~Item();

	virtual void tick(const sf::Time& deltaTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) override;
	virtual void setRenderLayer(int layer) override;
	virtual int getRenderLayer() const override;

	ItemDatabase::ItemStruct* getItemInfo();
	void anchorToEntity(Entity* entity);
	Entity* getAnchor() const;
	int getStackSize() const;
	void addToStack(int size);
	void setMaxStack();
	void setDrawMe(bool toDraw);
	int getInventorySlot() const;
	void setInventorySlot(int slot);

private:
	void setShitUp();

	Entity* mAnchor;
	sf::Sprite mSprite;
	sf::Text mStackText;
	sf::Vector2f mVelocity;
	ItemDatabase::ItemStruct* mItemInfo;
	bool mDrawMe;
	int mStackSize;
	int minventorySlot;
};