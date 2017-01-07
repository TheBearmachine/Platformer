#ifndef _INCLUDED_EVENTOBSERVER_H_
#define _INCLUDED_EVENTOBSERVER_H_

namespace sf {
	class Event;
}

class EventObserver {
public:
	virtual void observe(const sf::Event& _event) = 0;
};

#endif // !_INCLUDED_EVENTOBSERVER_H_
