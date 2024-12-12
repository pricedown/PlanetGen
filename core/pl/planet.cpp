// Joseph Isaacs

#include "planet.h"

namespace pl {
	const int SEED_LOWER_BOUND = -10000;
	const int SEED_UPPER_BOUND = 10000;

	float Planet::normalizedToRadius(float normalizedAltitude) {
		return minRadius + normalizedAltitude * abs(maxRadius - minRadius);
	}

	float Planet::radiusToNormalized(float radius) {
		return (radius - minRadius) / abs(maxRadius - minRadius);
	}

	void Planet::randomizeSeed() {
		seed.x = ew::RandomRange(SEED_LOWER_BOUND, SEED_UPPER_BOUND);
		seed.y = ew::RandomRange(SEED_LOWER_BOUND, SEED_UPPER_BOUND);
		seed.z = ew::RandomRange(SEED_LOWER_BOUND, SEED_UPPER_BOUND);
	}


	Layer::Layer(float altitude, glm::vec3 color) {
		this->altitude = altitude;
		this->color = color;
	}


	float Waves::periodToSpeed(float period) {
		return 1 / (period / 2 * ew::PI);
	}

	float Waves::speedToPeriod(float speed) {
		return (2 * ew::PI) / speed;
	}
}