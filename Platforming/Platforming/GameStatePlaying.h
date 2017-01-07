#ifndef _INCLUDED_GAMESTATEPLAYING_H_
#define _INCLUDED_GAMESTATEPLAYING_H_

#include "GameState.h"

#include <vector>

class EventManager;
class CollisionManager;
class EntityManager;
class Camera;
namespace sf {
	class RenderWindow;
}

class GameStatePlaying: public GameState {
public:
	GameStatePlaying(sf::RenderWindow* window, EventManager* eventManager);
	~GameStatePlaying();

	void setup() override;
	std::vector<int> readMap(const char* file);
	void update(sf::Clock& clock) override;
	void render() override;
	void observe(const sf::Event& _event) override;
	void handleEvents() override;

private:
	EventManager* mEventManager;
	CollisionManager* mCollisionManager;
	EntityManager* mEntityManager;
	sf::RenderWindow* mWindow;
	Camera* mCamera;

	unsigned mMapHeight;
	unsigned mMapWidth;
	bool mPaused;
};

#endif // !_INCLUDED_GAMESTATEPLAYING_H_

