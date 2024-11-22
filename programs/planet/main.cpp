#pragma once

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "pl/shader.h"
#include "pl/texture2d.h"
#include "pl/camera.h"
#include "pl/geometry.h"
#include "pl/noise.h"
#include "pl/mesh.h"
#include "pl/lighting.h"

#include "pl/circle.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

pl::Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

int main() {
	#pragma region Initialization
	printf("Initializing...");
	GLFWwindow* window;
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); //Unlocks mouse
	#pragma endregion
	#pragma region Geometry data
	pl::Mesh boxMesh(pl::cubeVertices, sizeof(pl::cubeVertices));
	#pragma endregion
	#pragma region World
	pl::initCubes();
	#pragma endregion

	pl::Shader boxShader = pl::Shader("assets/shaders/noiseVertexShader.vert", "assets/shaders/cube.frag");
	pl::Shader lightShader = pl::Shader("assets/shaders/light.vert", "assets/shaders/light.frag");
	pl::Texture2D container = pl::Texture2D("assets/textures/container.jpg", GL_LINEAR, GL_REPEAT);

	glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -2.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float ambientK = 0.1f, specularK = 0.5f, diffuseK = 0.5f;
	float shininess = 18.0f;
	bool blinnPhong = true;

	//jon code
	std::vector<pl::Vertex> vertices;
	std::vector<unsigned int> indices;

	/*vertices.push_back(pl::Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(0, 0)));
	vertices.push_back(pl::Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(1, 0)));
	vertices.push_back(pl::Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(1, 1)));
	vertices.push_back(pl::Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0, 0, 1), glm::vec2(0, 1)));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);*/
	pl::createSphere(2.0f, 256, vertices, indices);
	pl::Mesh sphere(vertices,indices);
	

	while (!glfwWindowShouldClose(window)) {
		// Inputs
		glfwPollEvents();

		// Update
		float time = glfwGetTime();
		deltaTime = time - lastFrame;
		lastFrame = time;

		camera.use(window);
		camera.timeChange(time);

		glm::mat4 projection = camera.projection();
		glm::mat4 view = camera.viewLookAt();

		// Draw
		// background
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		// light box
		lightShader.use();
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		lightShader.setVec3("lightPos", lightPos);
		lightShader.setVec3("lightColor", lightColor);
		boxMesh.DrawArray(lightShader);

		// boxes
		glEnable(GL_DEPTH_TEST);

		boxShader.use();
		boxShader.setVec3("viewPos", camera.getPosition());
		boxShader.setBool("blinnPhong", blinnPhong);
		boxShader.setVec3("lightPos", lightPos);
		boxShader.setVec3("lightColor", lightColor);
		boxShader.setFloat("ambientStrength", ambientK);
		boxShader.setFloat("diffuseStrength", diffuseK);
		boxShader.setFloat("specularStrength", specularK);
		boxShader.setFloat("shininess", shininess);
		container.Bind(GL_TEXTURE0);

		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);

		boxShader.setMat4("projection", projection);
		boxShader.setMat4("view", view);

		for (unsigned int i = 0; i < pl::CUBENUM; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pl::cubePositions[i]);
			model = glm::rotate(model, pl::cubeRotationAngles[i], pl::cubeRotations[i]);
			model = glm::scale(model, pl::cubeScales[i]);
			boxShader.setMat4("model", model);
			boxMesh.DrawArray(boxShader);
		}

		//Draw plane
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0.0, 0.0, -5.0));
		boxShader.setMat4("model", transform); 

		sphere.Draw(boxShader);


		/*
		// plane
		float pScale = 3.0f;
		pl::Mesh plane = pl::plane(pScale, pScale, 3);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0-0.5*pScale, 0-0.5*pScale, -3.0f));
		//model = glm::scale(model, glm::vec3(1.0f));
		boxShader.setMat4("model", model);
		plane.Draw(boxShader);
		*/

#pragma region ImGui
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Lighting");
		ImGui::Checkbox("Blinn-Phong", &blinnPhong);
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &shininess, 2.0f, 1024.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

