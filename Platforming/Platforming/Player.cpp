#include "Player.h"
#include "VectorMath.h"
#include "MouseCursor.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Clock.hpp>

static const float SPEED = 125.0f;
static const float TERMINAL_VELOCITY_X = 3.0f;
static const float ACCELLERATION = 2.7f;
static const float TERMINAL_VELOCITY_Y = 4.0f;
static const float JUMP_SPEED = 3.5f;
static const float GRAVITY = 5.0f;

Player::Player(sf::RenderWindow* window, EntityManager* entityManager) :
	mWindow(window), mEntityManager(entityManager),
	mCollisions(0), mCollisionVector(),
	mCanJump(false), mFriction(4.0) {
	mSprite.setFillColor(sf::Color::Blue);
	mSprite.setOutlineThickness(0.0f);
	setSprite(25.0f, 50.0f);
	setPosition((float)mWindow->getSize().x / 2, -400.0f);
	mCat = CollideCategory::PLAYER;
}

Player::~Player() {

}

void Player::setCursor(MouseCursor * cursor) {
	mCursor = cursor;
}

void Player::tick(const sf::Time & deltaTime) {
	//Shitty lag solution
	if (deltaTime.asSeconds() >= 0.75f)
		return;

	mCollisions = 0;
	mCollisionVector = { 0.0f, 0.0f };
	mWallCollides.clear();

	// Calculate movement
	mVelocity.y = mVelocity.y + GRAVITY * deltaTime.asSeconds();
	mVelocity.y = std::max(mVelocity.y, -TERMINAL_VELOCITY_Y);
	mVelocity.y = std::min(mVelocity.y, TERMINAL_VELOCITY_Y);


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		mVelocity.x = mVelocity.x - ACCELLERATION * deltaTime.asSeconds();
		mVelocity.x = std::max(mVelocity.x, -TERMINAL_VELOCITY_X);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		mVelocity.x = mVelocity.x + ACCELLERATION * deltaTime.asSeconds();
		mVelocity.x = std::min(mVelocity.x, TERMINAL_VELOCITY_X);
	}
	else if (mCanJump) {
		mVelocity.y = 0.0f;
		if (mVelocity.x < 0) {
			mVelocity.x = mVelocity.x + mFriction * deltaTime.asSeconds();
			mVelocity.x = std::min(mVelocity.x, 0.0f);
		}
		else if (mVelocity.x > 0) {
			mVelocity.x = mVelocity.x - mFriction * deltaTime.asSeconds();
			mVelocity.x = std::max(mVelocity.x, 0.0f);
		}
	}
	if (mCanJump && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		mCanJump = false;
		mVelocity.y = -JUMP_SPEED;
	}

	move(mVelocity * deltaTime.asSeconds() * SPEED);
	mCanJump = false;
}

void Player::collide(CollidableEntity* collidable, const sf::Vector2f& moveAway) {
	if (collidable->getCategory() == CollidableEntity::CollideCategory::WALL) {
		mCollisions++;
		mCollisionVector += moveAway;
		mWallCollides.push_back(collidable);

		move(moveAway);

		if (mCollisions == 2) {
			if ((mWallCollides[0]->getPosition().x > getPosition().x &&
				mWallCollides[1]->getPosition().x > getPosition().x) || (
					mWallCollides[0]->getPosition().x < getPosition().x &&
					mWallCollides[1]->getPosition().x < getPosition().x)) {
				move(0.0f, -mCollisionVector.y);
				mVelocity.x = 0.0f;
			}
			else if ((mWallCollides[0]->getPosition().y > getPosition().y &&
				mWallCollides[1]->getPosition().y > getPosition().y) || (
					mWallCollides[0]->getPosition().y < getPosition().y &&
					mWallCollides[1]->getPosition().y < getPosition().y)) {
				mCanJump = true;
				move(-mCollisionVector.x, 0.0f);
				mVelocity.y = 0.0f;
			}
		}
		else {

		}
	}
}
