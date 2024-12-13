// Joseph Isaacs
#include <glm/glm.hpp>

namespace pl {
	struct Light {
		glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 rimColor = color;
		float ambientK = 0.1f, specularK = 0.5f, diffuseK = 0.5f, rimK = 0;
		float shininess = 18.0f;
		float rimFalloff = 0.6;
		bool blinnPhong = true;
	};
} // namespace pl
