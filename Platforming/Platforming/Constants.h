#pragma once

#include <SFML/System/Vector2.hpp>

namespace Constants {
	namespace Block {
		static const int Height = 32;
		static const int Width = 32;
	}
	namespace Map {
		static const int Width = 512;
		static const int Height = 128;
	}
	namespace Physics {
		static const float Gravity = 5.0f;
		static const float ImpactBounce = 0.33f;
		static const sf::Vector2f TerminalVelocity = { 3.0f, 4.0f };
	}
	namespace Files {
		static const char* Default_Font = "Resources/Fonts/Candara.ttf";
	}
}