#ifndef _INCLUDED_GAMEMANAGER_H_
#define _INCLUDED_GAMEMANAGER_H_
#include "EventObserver.h"
#include <SFML/System/Clock.hpp>

namespace sf {
	class RenderWindow;
}

class GameState;
class EventManager;

class GameManager : public EventObserver{
public:
	GameManager();
	~GameManager();
	void run();
	virtual void observe(const sf::Event& _event) override;

private:
	bool initalizeGame();
	bool initalizeWindow();
	void close();
	void gameLoop();

	sf::Clock mClock;
	sf::RenderWindow* mWindow;
	GameState* mGameState;
	EventManager* mEventManager;
};

#endif // !_INCLUDED_GAMEMANAGER_H_