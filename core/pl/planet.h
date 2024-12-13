// Joseph Isaacs

#pragma once

#include <glm/glm.hpp>
#include "../ew/ewMath/ewMath.h"

namespace pl {

struct Planet {
    float minRadius = 1.5f;         // lowest depth
    float maxRadius = 2.5f;         // highest mountain height
    float mountainRoughness = 1.0f; // exponent of mountain noise
    float mountainFrequency = 3.0f; // frequency of mountain noise
    float waterLevel = 2.0f;        // scale of the water sphere, base of reference for Layers
    glm::vec3 seed = glm::vec3(0.0f, 0.0f, 0.0f);

    void randomizeSeed();

    float normalizedToRadius(float normalizedAltitude);
    float radiusToNormalized(float radius);
};

// Treated like a ColorStop in shaders
struct Layer {
    float altitude; // real scale relative to water radius
    glm::vec3 color;

    Layer(float altitude, glm::vec3 color);
};

struct Waves {
    float amplitude = 0.007f;
    float frequency = 19.0f;
    float speed = periodToSpeed(2.0f);

    float periodToSpeed(float period);
    float speedToPeriod(float speed);
};

} // namespace pl
