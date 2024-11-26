#include "circle.h"
#include <stdlib.h>

namespace pl {

	void createSphere(float radius, int subDivisions, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
		vertices.clear();
		indices.clear();
		vertices.reserve((subDivisions + 1) * (subDivisions + 1));
		indices.reserve(subDivisions * subDivisions * 6);

		float thetaStep = 2 * 3.14159265f / subDivisions;
		float phiStep = 3.14159265f / subDivisions;
		for (size_t row = 0; row <= subDivisions; row++)
		{
			float phi = row * phiStep;
			for (size_t col = 0; col <= subDivisions; col++)
			{
				float theta = col * thetaStep;
				glm::vec2 uv;
				uv.x = 1.0 - ((float)col / subDivisions);
				uv.y = 1.0 - ((float)row / subDivisions);
				glm::vec3 pos;
				pos.x = cosf(theta) * sinf(phi) * radius;
				pos.y = cosf(phi) * radius;
				pos.z = sinf(theta) * sinf(phi) * radius;
				glm::vec3 normal = glm::normalize(pos);
				vertices.emplace_back(pos, normal, uv);
			}
		}

		//Indices
		for (size_t row = 0; row < subDivisions; row++)
		{
			for (size_t col = 0; col < subDivisions; col++)
			{
				unsigned int bl = row * (subDivisions + 1) + col;
				unsigned int br = bl + 1;
				unsigned int tl = bl + subDivisions + 1;
				unsigned int tr = tl + 1;
				//Triangle 1
				indices.emplace_back(bl);
				indices.emplace_back(br);
				indices.emplace_back(tr);
				//Triangle 2
				indices.emplace_back(tr);
				indices.emplace_back(tl);
				indices.emplace_back(bl);
			}
		}
		return;
	}

  pl::Mesh createSphere(float radius, int subDivisions) {
    std::vector<pl::Vertex> vertices;
    std::vector<unsigned int> indices;

    pl::createSphere(radius, subDivisions, vertices, indices);
    return pl::Mesh(vertices, indices);
  }
}
