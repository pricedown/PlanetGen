#include <glm/glm.hpp>

namespace pl {

struct Planet {
    float minRadius = 1.5f; // scale of the planet mesh & denominator for mountain height
    float maxRadius = 2.5f; // max mountain height
    float mountainRoughness = 1.0f; // exponent to noise
    float waterLevel = 2.0f; // scale of the water mesh
};

} // namespace pl
