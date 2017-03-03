#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>

class Item;

class InventorySlot : public Entity {
public:
	InventorySlot(int index);
	~InventorySlot();

	void setTexture(const char* filename);
	Item* getContent();
	void setContent(Item* item);
	void setIndex(int index);
	int getIndex() const;

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;

private:
	sf::Sprite mBackground;
	Item* mContent;

	int mIndex;
	int mRenderLayer;
	bool mGarbage;
};