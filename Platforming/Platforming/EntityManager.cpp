#include "EntityManager.h"
#include "Entity.h"
#include "ItemManager.h"
#include <map>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/View.hpp>

EntityManager::EntityManager() :
	mEntities() {

}

EntityManager::~EntityManager() {
	internalCleanup();
}

void EntityManager::addEntity(Entity* entity) {
	mEntities.push_back(entity);
}

void EntityManager::updateEntities(const sf::Time& deltaTime) {
	std::vector<Entity*> temp(mEntities);
	for (auto it : temp) {
		it->tick(deltaTime);
	}
}

void EntityManager::renderElements(sf::RenderWindow & window) {
	std::map<int, std::vector<Entity*>> mappy;
	for (auto e : mEntities) {
		mappy[e->getRenderLayer()].push_back(e);
	}
	sf::View prevView = window.getView();
	sf::View GUIView = sf::View(sf::FloatRect(0, 0, (float)window.getSize().x, (float)window.getSize().y));
	bool drawGUI = false;
	for (auto it : mappy) {
		if (drawGUI || it.first >= 100) {
			if (drawGUI == false)
				window.setView(GUIView);
			drawGUI = true;
			for (auto itt : it.second) {
				window.draw(*itt);
			}
		}
		else {
			for (auto itt : it.second) {
				window.draw(*itt);
			}
		}
	}
	window.setView(prevView);
}

void EntityManager::garbageCollection() {
	std::vector<Entity*> temp;
	for (auto e : mEntities) {
		if (e->garbage())
			delete e;
		else
			temp.push_back(e);
	}
	mEntities = temp;
}

void EntityManager::internalCleanup() {
	while (!mEntities.empty()) {
		delete mEntities.back();
		mEntities.pop_back();
	}
}
