#ifndef _INCLUDED_COLLIDABLEENTITYDEFAULT_H_
#define _INCLUDED_COLLIDABLEENTITYDEFAULT_H_

#include "CollidableEntity.h"
#include <SFML/Graphics/RectangleShape.hpp>

class CollidableEntityDefault : public CollidableEntity {
public:
	CollidableEntityDefault(EntityType entityType);
	~CollidableEntityDefault();

	virtual void tick(const sf::Time & deltaTime) = 0;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override;
	virtual void kill() override;

	virtual void setSprite(sf::Vector2f size);
	virtual void setSprite(float width, float height);
	virtual CollideCategory getCategory() const override;
	virtual std::vector<sf::Vector2f> getAxes() const override;
	virtual void collide(CollidableEntity * collidable, const sf::Vector2f & moveAway) = 0;
	virtual sf::FloatRect getHitbox() const override;
	virtual sf::Shape * getNarrowHitbox() const override;

protected:
	virtual void updateAxes();

	bool mGarbage;
	sf::RectangleShape mSprite;
	sf::Shape* mHitboxShape;
	CollideCategory mCat;
	std::vector<sf::Vector2f> mAxes;
};

#endif // !_INCLUDED_COLLIDABLEENTITYDEFAULT_H_
