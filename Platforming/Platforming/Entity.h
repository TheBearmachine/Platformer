#ifndef _INCLUDED_ENTITY_H_
#define _INCLUDED_ENTITY_H_

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

namespace sf {
	class Time;
}

class Entity : public sf::Drawable, public sf::Transformable{
public:
	Entity();
	virtual ~Entity();

	virtual void tick(const sf::Time& deltaTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	virtual bool garbage() const = 0;
	virtual void kill() = 0;
};

#endif // !_INCLUDED_ENTITY_H_
