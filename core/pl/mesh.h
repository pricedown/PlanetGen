#pragma once

#include "../ew/external/glad.h"
#include "../ew/external/stb_image.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <ostream>

#include "shader.h"

using std::vector;
using std::string;

namespace pl {
	struct Vertex {
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 uv;

	};
	
	struct Texture {
		unsigned int id;
		string type;
	};

	class Mesh {
	public:
		// mesh data
		vector<Vertex>       vertices;
		vector<unsigned int> indices;
		vector<Texture>      textures;

		Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
		Mesh(vector<Vertex> vertices, vector<unsigned int> indices);
		Mesh(vector<Vertex> vertices);
		Mesh(const float vertexArray[], int arrayLength);
		void Draw(Shader& shader);
		void DrawArray(Shader& shader);
	private:
		//  render data
		unsigned int VAO, VBO, EBO;

		void setupVAOEBO();
		void setupVAO();
	};
};
