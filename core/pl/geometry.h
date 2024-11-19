#include <vector>
#include <glm/glm.hpp>
#include "../core/ew/ewMath/ewMath.h"
#include "mesh.h"

namespace pl {
	const float quadVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	};
	const float cubeVertices[] = {
		// position				normal				uv
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
	};
	const unsigned int CUBENUM = 20;
	glm::vec3 cubePositions[CUBENUM];
	glm::vec3 cubeScales[CUBENUM];
	glm::vec3 cubeRotations[CUBENUM];
	glm::vec3 cubeRotationDirs[CUBENUM];
	float cubeRotationAngles[CUBENUM];
	float cubeRotationSpeed = 0.7f;

	void initCubes() {
		for (int i = 0; i < CUBENUM; i++) {
			float cubeBounds = 6.0f;
			float fieldStartsAt = -0.0f;
			cubePositions[i].x = ew::RandomRange(-cubeBounds, cubeBounds);
			cubePositions[i].y = ew::RandomRange(-cubeBounds, cubeBounds);
			cubePositions[i].z = ew::RandomRange(fieldStartsAt + cubeBounds / 2.0f, fieldStartsAt - cubeBounds / 2.0f);

			float cubeMinScale = 0.25f;
			float cubeMaxScale = 1.2f;
			cubeScales[i] = glm::vec3(ew::RandomRange(cubeMinScale, cubeMaxScale));

			float cubeRotationAngle = ew::RandomRange(0.0f, 3.0f);
			cubeRotations[i] = cubeRotationAngle * glm::vec3(ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f));
			cubeRotationDirs[i] = glm::vec3(ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f), ew::RandomRange(-1.0f, 1.0f));
			cubeRotationAngles[i] = 0.0f;
		}
	}

	pl::Mesh plane(float width, float height, int subdiv) {
		std::vector<pl::Vertex> vertices;
		for (int row = 0; row <= subdiv; row++) {
			for (int col = 0; col <= subdiv; col++) {
				pl::Vertex v;

				v.pos.x = width * ((float)col / subdiv);
				v.pos.y = height *((float)row / subdiv);
				v.pos.z = 0.0;

				v.norm.x = 0;
				v.norm.y = 0;
				v.norm.z = 1.0;

				v.uv.x = (float)col / subdiv;
				v.uv.y = (float)row / subdiv;

				vertices.push_back(v);
			}
		}

		std::vector<unsigned int> indices;
		for (int row = 0; row < subdiv; row++) {
			for (int col = 0; col < subdiv; col++) {
				unsigned int bl = row * (subdiv + 1) + col;
				unsigned int br = bl + 1;
				unsigned int tl = bl + subdiv + 1;
				unsigned int tr = tl + 1;

				// counter clockwise winding order means it's facing us
				indices.push_back(bl);
				indices.push_back(br);
				indices.push_back(tr);
				indices.push_back(tr);
				indices.push_back(tl);
				indices.push_back(bl);
			}
		}

		pl::Mesh mesh = pl::Mesh(vertices, indices);
		return mesh;
	}

};