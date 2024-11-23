#pragma once

#ifndef NOISE_H
#define NOISE_H
#define _USE_MATH_DEFINES

#include "../ew/external/glad.h"

#include <glm/glm.hpp>
#include <random>
#include <ctime>
#include <cmath>

namespace pl {
	struct LatticeSpan4 {
		public:
			glm::ivec4 p0, p1;
			glm::vec4 g;
			glm::vec4 t;
	};
	class Noise {
		public:
			Noise();
			Noise(int p, int no);
			float noise();
			float noise(int min, int max);
			float interpolate(float from, float to, float mix);
			float smoothNoise(float x, float y);
			float interpolateNoise(float x, float y);
			float perlinNoise2D(float x, float y);
		
		private:
			int persistence;
			int numOctaves;
	};
#endif
}