#include "Camera.h"
#include "Player.h"
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

Camera::Camera(sf::RenderWindow* window):
mWindow(window){
	/*sf::View view;
	view.setSize(640, 480);
	mWindow->setView(view);*/
}

Camera::~Camera() {

}

bool Camera::setup(Entity* player, sf::Vector2u mapBounds) {
	if (player == nullptr)
		return false;

	mPlayer = player;
	mBounds = mapBounds;

	return true;
}

void Camera::tick(const sf::Time & deltaTime) {
	sf::View view = mWindow->getView();
	sf::Vector2f playerPos = mPlayer->getPosition();
	view.setCenter(playerPos);

	mWindow->setView(view);
}

void Camera::draw(sf::RenderTarget & target, sf::RenderStates states) const {

}
