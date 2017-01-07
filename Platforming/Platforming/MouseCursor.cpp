#include "MouseCursor.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "EventManager.h"

static const char* TEXTURE = "Images/MousePointer.png";

MouseCursor::MouseCursor():
mTexture(), mSprite(){
	mCat = CollideCategory::CURSOR;
	mTexture.loadFromFile(TEXTURE);
	mSprite.setTexture(mTexture);
	mHitboxShape->setTextureRect(sf::IntRect(-4, -4, 8, 8));
}

MouseCursor::~MouseCursor() {

}

void MouseCursor::initalize(Entity* player, EventManager* eventManager, sf::RenderWindow* window) {
	mPlayer = player;
	mEventManager = eventManager;
	mWindow = window;
	mEventManager->registerObserver(this, sf::Event::MouseMoved);
}

void MouseCursor::finalize(){
	mEventManager->unregisterObserver(this, sf::Event::MouseMoved);
}

void MouseCursor::tick(const sf::Time& deltaTime) {

}

void MouseCursor::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

void MouseCursor::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {

}

void MouseCursor::observe(const sf::Event& _event) {
	switch (_event.type) {
	case sf::Event::MouseMoved:
		setPosition((float)_event.mouseMove.x, (float)_event.mouseMove.y);
		break;
	}
}
