#ifndef _INCLUDED_CAMERA_H_
#define _INCLUDED_CAMERA_H_

#include "Entity.h"

namespace sf {
	class RenderWindow;
}

class Camera : public Entity {
public:
	Camera(sf::RenderWindow* window);
	~Camera();
	bool setup(Entity* player, sf::Vector2f mapBounds);

	virtual void tick(const sf::Time & deltaTime) override;
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	virtual bool garbage() const override { return false; };
	virtual void kill() override {};

private:
	Entity* mPlayer;
	sf::RenderWindow* mWindow;
	sf::Vector2f mBounds;
};
#endif // !_INCLUDED_CAMERA_H_
