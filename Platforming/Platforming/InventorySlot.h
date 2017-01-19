#pragma once

#include "Entity.h"
#include <SFML/Graphics/Sprite.hpp>

class Item;

class InventorySlot : public Entity {
public:
	InventorySlot();
	~InventorySlot();

	void setTexture(const char* filename);

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;

private:
	sf::Sprite mBackground;
	//sf::Sprite mContentIcon;
	Item* mContent;

	bool mGarbage;
};