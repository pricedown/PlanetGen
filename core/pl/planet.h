#include <glm/glm.hpp>

namespace pl {

struct Planet {
    float minRadius = 2.0f; // scale of the planet mesh & denominator for mountain height
    float maxRadius = 3.0f; // max mountain height
    float waterLevel = 2.5f; // scale of the water mesh
    float mountainRoughness = 1.0f; // exponent to noise
};

} // namespace pl
