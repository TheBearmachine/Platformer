#pragma once

#include <SFML/System/Vector2.hpp>

namespace Constants {
	namespace Block {
		extern const int Height;
		extern const int Width;
	}
	namespace Map {
		extern const int Width;
		extern const int Height;
	}
	namespace Physics {
		extern const float Gravity;
		extern const float ImpactBounce;
		extern const sf::Vector2f TerminalVelocity;
	}
}