#include <vector>
#include <glm/glm.hpp>

namespace pl {

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 uv;
	};

	struct Mesh {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		void drawArray() {
			unsigned int VAO, VBO, EBO;

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void drawElements() {
			unsigned int VAO, VBO, EBO;

			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 6));
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	};

	Mesh plane(float width, float height, int subdiv) {
		std::vector<Vertex> vertices;
		for (int row = 0; row <= subdiv; row++) {
			for (int col = 0; col <= subdiv; col++) {
				Vertex v;

				v.pos.x = col / subdiv;
				v.pos.y = row / subdiv;
				v.pos.z = 0.0;

				v.norm.x = 0;
				v.norm.y = 0;
				v.norm.z = 1.0;
				
				v.uv.x = col / subdiv;
				v.uv.y = row / subdiv;

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
				/*
				indices.push_back(bl);
				indices.push_back(br);
				indices.push_back(tr);
				indices.push_back(tr);
				indices.push_back(tl);
				indices.push_back(bl);
				*/

				indices.push_back(bl);
				indices.push_back(br);
				indices.push_back(tl);
				indices.push_back(bl);
				indices.push_back(tr);
				indices.push_back(tl);
			}
		}

		Mesh mesh;
		mesh.indices = indices;
		mesh.vertices = vertices;
		return mesh;
	}

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
};