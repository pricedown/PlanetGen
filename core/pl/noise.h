#pragma once

#ifndef NOISE_H
#define NOISE_H
#define _USE_MATH_DEFINES

#include "../ew/external/glad.h"

#include <random>
#include <ctime>
#include <cmath>

namespace pl {
	class Noise {
	public:
		Noise();
		float noise();
		float noise(int min, int max);
		float interpolate(float from, float to, float mix);
		float smoothNoise(float x, float y);
		float interpolateNoise(float x, float y);
	};
#endif
}