#include "MapGenerator.h"
#include <cmath>
#include <vector>
#include <SFML/System/Vector2.hpp>

int permutation[] = { 151,160,137,91,90,15,										// Hash lookup table as defined by Ken Perlin.  This is a randomly
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,    // arranged array of all numbers from 0-255 inclusive.
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

int p[512];
float repeat = 0.0f;

void Perlin() {
	for (int i = 0; i < 512; i++) {

		p[i] = permutation[i % 256];
	}
}

float lerp(float w, float a, float b) {
	return a + w * (b - a);
}

float fade(float t) {
	return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

int inc(int n) {
	n++;
	if (repeat > 0)
		n = n % (int)repeat;
	return n;
}

float grad(int hash, float x, float y, float z) {
	switch (hash & 0xF) {
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0; // Shouldnt happend
	}
}

float perlin(float x, float y, float z) {
	if (repeat > 0.0f) {
		x = modff(x, &repeat);
		y = modff(y, &repeat);
		z = modff(z, &repeat);
	}

	int X = (int)std::floor(x) & 255;
	int Y = (int)std::floor(y) & 255;
	int Z = (int)std::floor(z) & 255;

	float xf = x - (int)std::floor(x);
	float yf = y - (int)std::floor(y);
	float zf = z - (int)std::floor(z);

	float u = fade(xf);
	float v = fade(yf);
	float w = fade(zf);

	int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,
		B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;
	return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
		grad(p[BA], x - 1, y, z)),
		lerp(u, grad(p[AB], x, y - 1, z),
			grad(p[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
			grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
}

float noise(float nx, float ny) {
	return perlin(nx, ny, 0) / 2.0f + 0.5f;
}

float noise2(float x, float y) {
	int n = (int)x + (int)y * 57;
	n = (n << 13) ^ n;
	int nn = (n*(n*n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0 - ((float)nn / 1073741824.0);
}

void MapGenerator::generateMap(int * arr, int width, int height) {
	Perlin();
	float frequency = 15.0f;
	float maxHeight = 0.0f,
		minHeight = 0.0f;
	float maxPerlin = 0.0f,
		minPerlin = 0.0f;
	for (int i = 0; i < width; i++) {
		float nx = (float)i *0.15f + frequency;
		float heightMap = noise(nx, 1.0f); // ~ -6 to 7
		//heightmap query start
		if (heightMap < minHeight) minHeight = heightMap;
		else if (minHeight == 0.0f && maxHeight == 0.0f) minHeight = maxHeight = heightMap;
		if (heightMap > maxHeight) maxHeight = heightMap;
		//heightmap query end
		for (int j = 0; j < height; j++) {
			float ny = (float)j * 0.15f + frequency;
			if (heightMap >= ny - 2.0f) {
				arr[i + j * width] = -1;
			}
			else {
				float temp = noise(nx + 10.0f, ny + 10.0f); //~ -20 to 22
				float temp2 = noise(nx + 154.0f, ny + 884.0f);

				//perlin query start
				if (temp < minPerlin) minPerlin = temp;
				else if (maxPerlin == 0.0f && minPerlin == 0.0f) minPerlin = maxPerlin = temp;
				if (temp > maxPerlin) maxPerlin = temp;
				//perlin query end
				//temp = 0.0f;
				if (temp <= 0.3f) {
					arr[i + j * width] = -1;
				}
				else
					if (temp2 < 0.5) {
						arr[i + j * width] = 0;
					}
					else {
						arr[i + j * width] = 10;

					}
			}
		}
	}
}
