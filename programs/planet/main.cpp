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
#include "pl/mesh.h"
#include "pl/noise.h"
#include "pl/planet.h"

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

	pl::Shader planetShader = pl::Shader("assets/shaders/noise.vert", "assets/shaders/noise.frag");
	pl::Shader waterShader = pl::Shader("assets/shaders/water.vert", "assets/shaders/water.frag");
	pl::Shader lightShader = pl::Shader("assets/shaders/light.vert", "assets/shaders/light.frag");
	pl::Texture2D container = pl::Texture2D("assets/textures/container.jpg", GL_LINEAR, GL_REPEAT);

  pl::Light light;
	light.pos = glm::vec3(0.0f, 0.0f, 4.0f);
	light.ambientK = 0.8f;
  light.diffuseK = 0.8f;

  pl::Light waterLight;
	waterLight.pos = glm::vec3(0.0f, 0.0f, 4.0f);
	waterLight.specularK = 1.0f;

  pl::Planet planetTopology;

	//jon code

  pl::Mesh planet = pl::createSphere(planetTopology.minRadius, 256);
  pl::Mesh water = pl::createSphere(planetTopology.minRadius + planetTopology.waterLevel, 256);

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
		lightModel = glm::translate(lightModel, light.pos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		lightShader.setVec3("lightPos", light.pos);
		lightShader.setVec3("lightColor", light.color);
		boxMesh.DrawArray(lightShader);

		// planet
		glEnable(GL_DEPTH_TEST);

		planetShader.use();

		planetShader.setVec3("viewPos", camera.getPosition());
		planetShader.setFloat("minRadius", planetTopology.minRadius);
		planetShader.setFloat("maxRadius", planetTopology.maxRadius);
    planetShader.setLight(light);
		container.Bind(GL_TEXTURE0);

		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);

		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);

		glm::mat4 ptransform = glm::mat4(1.0f);
		ptransform = glm::translate(ptransform, glm::vec3(0.0, 0.0, -5.0));
		planetShader.setMat4("model", ptransform); 

		planet.Draw(planetShader);

    // water
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
    waterShader.use();

		waterShader.setVec3("viewPos", camera.getPosition());
    waterShader.setLight(waterLight);
    waterShader.setVec3("waterColor", glm::vec3(0.0f,0.0f,1.0f)); // TODO: formalize
    waterShader.setFloat("waterAlpha", 0.3f); // TODO: formalize
		container.Bind(GL_TEXTURE0);

		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);

		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);

		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, glm::vec3(0.0, 0.0, -5.0));
		waterShader.setMat4("model", transform); 

		water.Draw(planetShader);


#pragma region ImGui
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Lighting");
		ImGui::Checkbox("Blinn-Phong", &light.blinnPhong);
		ImGui::DragFloat3("Light Position", &light.pos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &light.color.r);
		ImGui::SliderFloat("Ambient K", &light.ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &light.diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &light.specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &light.shininess, 2.0f, 1024.0f);
		ImGui::End();

		ImGui::Begin("Planet");
		ImGui::SliderFloat("Water Level", &planetTopology.waterLevel, 0.05f, 3.0f);
		ImGui::SliderFloat("Lowest Depth", &planetTopology.minRadius, 0.1f, 3.0f);
		ImGui::SliderFloat("Highest Peak", &planetTopology.maxRadius, 0.1f, 3.0f);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

