#include "MapGenerator.h"
#include "simplexnoise.h"
#include <cmath>
#include <vector>
#include <SFML/System/Vector2.hpp>

float noise(float nx, float ny) {

	return (raw_noise_2d(nx, ny) + 1.0f) / 2.0f;
}

float fBm(float x, float y, int octaves, float lacunarity, float gain) {
	float amp = 1;
	sf::Vector2f vec(x, y);
	float sum = 0;
	for (int i = 0; i < octaves; i++) {
		sum += amp * raw_noise_2d(vec.x, vec.y);
		amp *= gain;
		vec *= lacunarity;
	}
	return sum;
}

float RidgedMultifractal(float x, float y, int octaves, float lacunarity, float gain, float H, float sharpness, float threshold) {
	float result, signal, weight, exponent;
	sf::Vector2f vec(x, y);

	for (int i = 0; i < octaves; i++) {
		if (i == 0) {
			signal = raw_noise_2d(vec.x, vec.y);
			if (signal <= 0.0)
				signal = -signal;
			signal = gain - signal;
			signal = pow(signal, sharpness);
			result = signal;
			weight = 1.0f;
		}
		else {
			exponent = pow(lacunarity, -i*H);
			vec *= lacunarity;
			weight = signal * threshold;
			weight = std::fminf(std::fmaxf(weight, 0.0f), 1.0f);
			signal = raw_noise_2d(vec.x, vec.y);
			signal = abs(signal);
			signal *= weight;
			result += signal * exponent;
		}
	}
	return result;
}

float turbulence(float x, float y, int octaves, float lacunarity, float gain) {
	float amp = 1;
	sf::Vector2f vec(x, y);
	float sum = 0;
	for (int i = 0; i < octaves; i++) {
		sum += std::abs(amp * raw_noise_2d(vec.x, vec.y));
		amp *= gain;
		vec *= lacunarity;
	}
	return sum / 2.0f;
}

void MapGenerator::generateMap(int* arr, int width, int height) {
	float frequency = 15.0f;
	for (int i = 0; i < width; i++) {
		float nx = (float)i *0.15f + frequency;
		float heightMap = RidgedMultifractal(nx / 4.0f, 1.0f, 8, 2.5f, 0.8f, 0.8f, 2.0f, 2.0f) / 2.0f;
		for (int j = 0; j < height; j++) {
			float yCoord = (float(j) / float(height))*4.0f;
			float ny = (float)j * 0.15f + frequency;
			if (heightMap >= yCoord) {
				arr[i + j * width] = -1;
			}
			else {
				float temp = noise(nx + 10.0f, ny + 10.0f);
				float temp2 = noise(nx / 2.0f + 154.0f, ny / 2.0f + 884.0f);
				//float temp2 = turbulence(nx / 20.0f + 154.0f, ny / 20.0f + 884.0f, 4, 2.5f, 0.4f);
				if (temp2 <= 0.3f) {
					arr[i + j * width] = -1;
				}
				else if (temp < 0.5) {
					arr[i + j * width] = 0;
				}
				else {
					arr[i + j * width] = 10;
				}
			}
		}
	}
}
