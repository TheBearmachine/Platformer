#include "GameStatePlaying.h"
#include "EventManager.h"
#include "Player.h"
#include "TileMap.h"
#include "MouseCursor.h"
#include "Backdrop.h"
#include "MapGenerator.h" 
#include "Camera.h"
#include "EntityManager.h"
#include "CollisionManager.h"
#include <SFML/Graphics/RenderWindow.hpp>

static const char* TILESET = "Images/Tiles.png";
static const char* BACKGROUND = "Images/Backdrop.png";
static const char* MAP = "Images/Map.png";

static const int HEIGHT = 128;
static const int WIDTH = 512;

GameStatePlaying::GameStatePlaying(sf::RenderWindow* window, EventManager* eventManager) :
	mWindow(window), mEventManager(eventManager), mPaused(false),
	mEntityManager(new EntityManager()) {
	mCollisionManager = new CollisionManager(window);
	mEventManager->registerObserver(this, sf::Event::EventType::KeyPressed);
}

GameStatePlaying::~GameStatePlaying() {
	mEventManager->unregisterObserver(this, sf::Event::EventType::KeyPressed);
	delete mEntityManager;
}

std::vector<int> GameStatePlaying::readMap(const char* file) {
	std::vector<int> vec;
	sf::Image mapImage;
	if (!mapImage.loadFromFile(file))
		return vec;

	mMapWidth = mapImage.getSize().x;
	mMapHeight = mapImage.getSize().y;
	const sf::Uint8* pixelArray = mapImage.getPixelsPtr();
	const std::size_t arraySize = mMapHeight * mMapWidth * 4;

	for (std::size_t i = 0; i < arraySize; i += 4) {
		sf::Color pixel = { *(pixelArray + i), *(pixelArray + i + 1), *(pixelArray + i + 2), *(pixelArray + i + 3) };
		if (pixel == sf::Color(0, 0, 0))
			vec.push_back(-1);
		else if (pixel == sf::Color(127, 0, 127))
			vec.push_back(0);
		else if (pixel == sf::Color(0, 255, 0))
			vec.push_back(1);
		else if (pixel == sf::Color(76, 76, 76))
			vec.push_back(10);
	}
	return vec;
}

void GameStatePlaying::setup()
{
	MouseCursor* cursor = new MouseCursor();
	mEntityManager->addEntity(cursor, 100);
	mCollisionManager->addDynamicCollidable(cursor);

	Player* player = new Player(mWindow, mEntityManager);
	mEntityManager->addEntity(player, 50);
	mCollisionManager->addDynamicCollidable(player);
	std::vector<int> level = readMap(MAP);

	cursor->initalize(player, mEventManager, mWindow);
	player->setCursor(cursor);

	mMapHeight = HEIGHT;
	mMapWidth = WIDTH;
	int levelAlso[WIDTH * HEIGHT];
	MapGenerator::generateMap(levelAlso, mMapWidth, mMapHeight);
	TileMap* tileMap = new TileMap(mCollisionManager);
	if (!tileMap->load(TILESET, sf::Vector2u(32, 32), &levelAlso[0], mMapWidth, mMapHeight))
		return;
	mEntityManager->addEntity(tileMap, 2);

	/*Backdrop* backdrop = new Backdrop();
	backdrop->load(BACKGROUND);
	mEntityManager->addEntity(backdrop, 110);*/

	mCamera = new Camera(mWindow);
	mCamera->setup(player, sf::Vector2u(mMapWidth, mMapHeight));
	//mEntityManager->addEntity(camera, 1);
}

void GameStatePlaying::update(sf::Clock & clock) {
	sf::Time time = clock.restart();
	if (!mPaused) {
		mEntityManager->updateEntities(time);
		mCollisionManager->detectCollisions();
		//Camera has to update after collision in order not to stutter
		mCamera->tick(time);
		mCollisionManager->removeDeadCollidables();
		mEntityManager->garbageCollection();
	}
}

void GameStatePlaying::render() {
	mEntityManager->renderElements(*mWindow);
}

void GameStatePlaying::observe(const sf::Event& _event) {
	switch (_event.type) {
	case sf::Event::EventType::KeyPressed:
		if (_event.key.code == sf::Keyboard::P) {
			mPaused = !mPaused;
		}

	default:
		break;
	}
}

void GameStatePlaying::handleEvents() {
	sf::Event currEvent;
	while (mWindow->pollEvent(currEvent)) {
		mEventManager->notify(currEvent);
	}
}
