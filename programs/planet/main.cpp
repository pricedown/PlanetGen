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

#include "ccarreon/shader.h"
#include "ccarreon/texture2d.h"
#include "ccarreon/camera.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

float boxVertices[] = {
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,      0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,	1.0f, 0.0f,      0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,      0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,      0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,      0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 0.0f,      0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,      0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,      0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,      0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 1.0f,      0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,      0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,      0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	1.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,     -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,     -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	1.0f, 0.0f,     -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,      1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,      1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,      1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	0.0f, 1.0f,      1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	0.0f, 0.0f,      1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,      1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,      0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,	1.0f, 1.0f,      0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,      0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,	1.0f, 0.0f,      0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,	0.0f, 0.0f,      0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,	0.0f, 1.0f,      0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,      0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,	1.0f, 1.0f,      0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,	1.0f, 0.0f,      0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,	0.0f, 0.0f,      0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,	0.0f, 1.0f,      0.0f,  1.0f,  0.0f
};
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3  
};
glm::vec3 lightPos(0.0f, 5.0f, -5.0f);
glm::vec3 lightColor(1.0f, 0.0f, 0.0f);
float textureMix = 0.2f;
float ambientStrength = 0.1f;
float diffuseStrength = 1.0f;
float specularStrength = 0.5f;
int shininess = 32;
int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lighting", NULL, NULL);
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

	//Initialization goes here!
	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//LIGHT POSITION (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

	//NEWER OPTION:  glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//POSITION (XYZ)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//TEXTURE (UV)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//NORMALS (XYZ)
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(sizeof(float) * 5));
	glEnableVertexAttribArray(2);

	ccarreon::Shader boxShader("assets/boxVertexShader.vert", "assets/boxFragmentShader.frag");
	ccarreon::Texture2D boxTexture("assets/container.jpg", GL_LINEAR, GL_REPEAT);
	ccarreon::Texture2D faceTexture("assets/awesomeface.png", GL_LINEAR, GL_REPEAT);

	glEnable(GL_DEPTH_TEST);

	//PROJECTION
	glm::mat4 projection;

	ccarreon::Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	// Activate the shader
	boxShader.use();
	boxShader.setInt("texture1", 0);
	boxShader.setInt("texture2", 1);

	//MOUSE
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Render loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		float time = (float)glfwGetTime();

		//ImGUI start
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Settings window
		ImGui::Begin("Settings");
		ImGui::DragFloat3("Light Position", &lightPos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &lightColor.r);
		ImGui::SliderFloat("Ambient K", &ambientStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &diffuseStrength, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &specularStrength, 0.0f, 1.0f);
		ImGui::SliderInt("Shininess", &shininess, 2,1024);
		ImGui::SliderFloat("Texture Mix", &textureMix, 0.0f, 1.0f);
		ImGui::End();

		//Get user input
		camera.use(window);

		//Set projcective perspection
		camera.projectionSwap(projection);

		// Clear framebuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set uniforms
		boxShader.setFloat("uTime", time);
		boxShader.setFloat("textureMix", textureMix);
		boxShader.setFloat("ambientStrength", ambientStrength);
		boxShader.setFloat("diffuseStrength", diffuseStrength);
		boxShader.setFloat("specularStrength", specularStrength);
		boxShader.setInt("shininess", shininess);
		boxShader.setVec3("viewPos", camera.getPosition());
		boxShader.setVec3("lightPos", lightPos);
		boxShader.setVec3("lightColor", lightColor);
		boxShader.setBool("override", false);

		// Bind textures
		boxTexture.Bind(0);
		faceTexture.Bind(1);

		// View
		glm::mat4 view = glm::mat4(1.0f);
		camera.viewLookAt(view);

		// Lighting
		float specularStrength = 0.5;

		//Send matrices to shader
		unsigned int viewLoc = glGetUniformLocation(boxShader.ID, "view");
		unsigned int projectionLoc = glGetUniformLocation(boxShader.ID, "projection");

		float currentFrame = glfwGetTime();
		camera.timeChange(currentFrame);
		
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Update vertex buffer if necessary
		glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

		// Draw elements
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * (i + 1);
			model = glm::rotate(model, (float)glfwGetTime() + glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			unsigned int modelLoc = glGetUniformLocation(boxShader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// Make Light Cube
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		unsigned int modelLoc = glGetUniformLocation(boxShader.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lightModel));

		// Make light cube override color settings
		boxShader.setBool("override", true);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//ImGui render
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Swap buffers
		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}