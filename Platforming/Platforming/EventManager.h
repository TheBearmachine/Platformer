#ifndef _INCLUDED_EVENTMANAGER_H_
#define _INCLUDED_EVENTMANAGER_H_

#include <unordered_map>
#include <set>
#include <vector>
#include <SFML/Window/Event.hpp>

class EventObserver;

class EventManager {
public:
	EventManager();
	~EventManager();

	void registerObserver(EventObserver* observer, const std::vector<sf::Event::EventType>& types);
	void registerObserver(EventObserver* observer, const sf::Event::EventType& type);
	void unregisterObserver(EventObserver* observer, const std::vector<sf::Event::EventType>& types);
	void unregisterObserver(EventObserver* observer, const sf::Event::EventType& type);
	void notify(const sf::Event& _event) const;

private:
	typedef std::set<EventObserver*> ObserverSet;
	std::unordered_map<sf::Event::EventType, ObserverSet> mObservers;
};

#endif // !_INCLUDED_EVENTMANAGER_H_