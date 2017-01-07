#include "GameManager.h"
#include "EventManager.h"
#include "GameStatePlaying.h"

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

static const char* TITLE = "Burrowing Deeper";

GameManager::GameManager() :
	mClock(),
	mWindow(new sf::RenderWindow()), mEventManager(new EventManager())
{
	mGameState = new GameStatePlaying(mWindow, mEventManager);
	mEventManager->registerObserver(this, std::vector<sf::Event::EventType>{sf::Event::EventType::Closed, sf::Event::EventType::KeyPressed});
}

GameManager::~GameManager() {
	mEventManager->unregisterObserver(this, sf::Event::EventType::Closed);
}

void GameManager::run() {
	if (!initalizeGame()) {
		printf("Failed to initialize the game.");
		exit(-1);
	}
	gameLoop();
}

void GameManager::observe(const sf::Event & _event) {
	switch (_event.type) {
	case sf::Event::EventType::Closed:
		close();
		break;
	case sf::Event::EventType::KeyPressed:
		if (_event.key.code == sf::Keyboard::Escape) {
			close();
		}

	default:
		break;
	}
}

bool GameManager::initalizeGame() {
	if (!initalizeWindow()) {
		printf("Failed to initalize window");
		return false;
	}
	srand((unsigned int)time(NULL));
	return true;
}

bool GameManager::initalizeWindow() {
	sf::VideoMode video = sf::VideoMode::getDesktopMode();
	video.getDesktopMode();
	if (!video.isValid())
		return false;
	mWindow->create(video, TITLE, sf::Style::Fullscreen);
	if (!mWindow->isOpen())
		return false;
	mWindow->setMouseCursorVisible(false);
	mWindow->setVerticalSyncEnabled(false);

	return true;
}

void GameManager::close() {
	mWindow->close();
}

void GameManager::gameLoop() {
	mClock.restart();
	mGameState->setup();
	while (mWindow->isOpen()) {
		
		mGameState->handleEvents();
		mGameState->update(mClock);

		mWindow->clear();
		mGameState->render();
		mWindow->display();
	}
}

