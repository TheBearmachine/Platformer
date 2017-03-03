#include "Camera.h"
#include "Player.h"
#include "Constants.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Camera::Camera(sf::RenderWindow* window) :
	Entity(UNUSED),
	mWindow(window) {
	/*sf::View view;
	view.setSize(640, 480);
	mWindow->setView(view);*/
}

Camera::~Camera() {

}

bool Camera::setup(Entity* player, sf::Vector2f mapBounds) {
	if (player == nullptr)
		return false;

	mPlayer = player;
	mBounds = mapBounds;

	return true;
}

void Camera::tick(const sf::Time& deltaTime) {
	sf::View view = mWindow->getView();
	sf::Vector2f playerPos = mPlayer->getPosition();
	view.setCenter(playerPos);

	float halfWidth = view.getSize().x / 2.0f;
	float halfHeight = view.getSize().y / 2.0f;

	// Don't let the camera move out of bounds
	if (view.getCenter().x < halfWidth)
		view.setCenter(halfWidth, view.getCenter().y);
	else if (view.getCenter().x > mBounds.x - halfWidth)
		view.setCenter(mBounds.x - halfWidth, view.getCenter().y);

	if (view.getCenter().y < halfHeight)
		view.setCenter(view.getCenter().x, halfHeight);
	else if (view.getCenter().y > mBounds.y - halfHeight)
		view.setCenter(view.getCenter().x, mBounds.y - halfHeight);

	mWindow->setView(view);
}

void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const {

}
