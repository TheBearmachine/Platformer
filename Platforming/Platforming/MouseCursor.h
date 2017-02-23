#ifndef _INCLUDED_MOUSECURSOR_H_
#define _INCLUDED_MOUSECURSOR_H_

#include "CollidableEntityDefault.h"
#include "EventObserver.h"
#include "SFML/Graphics/Sprite.hpp"
#include <SFML/Graphics/Texture.hpp>

namespace sf {
	class RenderWindow;
}

class EventManager;
class Inventory;

class MouseCursor :public CollidableEntityDefault, public EventObserver {
public:
	MouseCursor();
	~MouseCursor();

	void initalize(EventManager* eventManager, sf::RenderWindow* window);
	void finalize();
	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual void collide(CollidableEntity * collidable, const sf::Vector2f & moveAway) override;
	virtual void observe(const sf::Event & _event) override;

private:
	sf::Sprite mSprite;
	sf::Texture mTexture;
	sf::RenderWindow* mWindow;
	EventManager* mEventManager;


};

#endif // !_INCLUDED_MOUSECURSOR_H_

