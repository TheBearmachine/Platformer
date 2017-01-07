#ifndef _INCLUDED_BACKDROP_H_
#define _INCLUDED_BACKDROP_H_

#include "Entity.h"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Backdrop : public Entity {
public:
	Backdrop();
	~Backdrop();
	bool load(const char* file);

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;

private:
	sf::Sprite mSprite;
	sf::Texture mTexture;
};

#endif // !_INCLUDED_BACKDROP_H_
