#pragma once

#include <glm/glm.hpp>
#include "../ew/ewMath/ewMath.h"

namespace pl {

struct Planet {
    float minRadius = 1.5f; // lowest depth
    float maxRadius = 2.5f; // max mountain height
    float mountainRoughness = 1.0f; // exponent to noise
    float waterLevel = 2.0f; // scale of the water sphere, used in Layer colors
    float mountainFrequency = 3.0f;
    glm::vec3 seed = glm::vec3(0.0f, 0.0f, 0.0f);

    float normalizedToRadius(float normalizedAltitude) {
        return minRadius + normalizedAltitude * abs(maxRadius - minRadius);
    }

    float radiusToNormalized(float radius) {
        return (radius - minRadius) / abs(maxRadius - minRadius);
    }

    void randomizeSeed() {
        seed.x = ew::RandomRange(-10000, 10000);
        seed.y = ew::RandomRange(-10000, 10000);
        seed.z = ew::RandomRange(-10000, 10000);
    }
};

// Treated as a ColorStop
// https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_images/Using_CSS_gradients
struct Layer {
    float altitude; // real scale relative to water radius
    glm::vec3 color;

    Layer(float altitude, glm::vec3 color) {
        this->altitude = altitude;
        this->color = color;
    }
};

} // namespace pl
