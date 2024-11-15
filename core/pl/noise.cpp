#include "noise.h"

using namespace pl;

Noise::Noise() {
	srand(time(NULL));
}

float Noise::noise() {
	return ((float)rand() / RAND_MAX) * 2 - 1;
	//https://www.sololearn.com/en/Discuss/280755/random-floats-in-c
}

float Noise::noise(int min, int max) {
	return ((float)rand() / RAND_MAX) * (max - min) + min;
	//https://www.sololearn.com/en/Discuss/280755/random-floats-in-c
}

float Noise::interpolate(float from, float to, float mix) {
	float ft = mix * M_PI;
	float f = (1.0 - cos(ft)) * 0.5;
	float out = from * (1.0 - f) + to * f;
	return out;
	//https://web.archive.org/web/20160530124230/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
}

float Noise::smoothNoise(float x, float y) {
	float corners = (noise(x - 1.0, y - 1.0) + noise(x + 1.0, y - 1.0) + noise(x - 1.0, y + 1.0) + noise(x + 1.0, y + 1.0)) / 16.0;
	float sides = (noise(x - 1.0, y) + noise(x + 1.0, y) + noise(x, y - 1.0) + noise(x, y + 1.0)) / 8.0;
	float center = noise(x, y) / 4.0;
	return corners + sides + center;
	//https://web.archive.org/web/20160530124230/http://freespace.virgin.net/hugo.elias/models/m_perlin.htm
}

float Noise::interpolateNoise(float x, float y) {
	int intX = int(x);
	int intY = int(y);
	float fracX = x - intX;
	float fracY = y - intY;
}