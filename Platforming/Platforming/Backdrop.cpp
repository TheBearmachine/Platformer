#include "Backdrop.h"
#include <SFML/Graphics/RenderWindow.hpp>

Backdrop::Backdrop() :
	Entity(UNUSED),
	mSprite(), mTexture() {

}

Backdrop::~Backdrop() {

}

bool Backdrop::load(const char* file) {
	if (!mTexture.loadFromFile(file))
		return false;

	mSprite.setTexture(mTexture);
	return true;
}

void Backdrop::tick(const sf::Time & deltaTime) {

}

void Backdrop::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	target.draw(mSprite, states);
}

bool Backdrop::garbage() const {
	return false;
}

void Backdrop::kill() {

}
