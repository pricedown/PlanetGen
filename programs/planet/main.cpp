#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <random>
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
#include "pl/planet.h"
#include "pl/cubemap.h"

const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void processInput(GLFWwindow* window, glm::mat4* model, pl::Camera cam);

glm::vec3 powColor(glm::vec3 color, float amount);

int main() {
#pragma region Initialization
	srand(time(NULL));

	printf("Initializing...");
	GLFWwindow* window;
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}
	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "PlanetGen", NULL, NULL);
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
	pl::Mesh planet = pl::createSphere(1.0, 256);
	pl::Mesh water = pl::createSphere(1.0, 256);

	pl::Mesh slight = pl::createSphere(1, 64);
#pragma endregion

#pragma region World
	pl::Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);

	pl::Planet planetTopology;
	pl::Waves waves;

	pl::Light planetLight;
	planetLight.ambientK = 0.375f;
	planetLight.pos = glm::vec3(0.0f, 0.0f, 20.0f);
	planetLight.rimK = 0.3;
	planetLight.rimFalloff = 1.2;

	glm::vec3 waterColor = glm::vec3(0.1, 0.1, 0.41);
	pl::Light waterLight;
	waterLight.pos = planetLight.pos;
	waterLight.specularK = 1.4f;
	waterLight.rimColor = glm::vec3(0.1,0.1,0.78);
	waterLight.diffuseK *= 0.9f;
	waterLight.shininess *= 1.3f;
	waterLight.rimK = 1.4;
	float waterAlpha = 0.3f;

	pl::Layer waterDeep = pl::Layer(-0.9, glm::vec3(0.05, 0.05, 1.0));
	pl::Layer waterShallow = pl::Layer(-0.1, glm::vec3(0.2, 0.2, 0.3));
	pl::Layer sand = pl::Layer(0, glm::vec3(0.79, 0.74, 0.57));
	pl::Layer land1 = pl::Layer(0.1, powColor(glm::vec3(0.333, 0.419, 0.184), 0.005) * glm::vec3(0.79, 0.74, 0.57) * glm::vec3(0.133, 0.530, 0.133));
	pl::Layer land2 = pl::Layer(0.25, glm::vec3(0.333, 0.419, 0.184)); // Land (more olive green)
	pl::Layer land3 = pl::Layer(0.3, glm::vec3(0.345, 0.471, 0.074));  // Land (brownish)
	pl::Layer snow1 = pl::Layer(0.35, glm::vec3(0.933, 0.933, 0.933)); // Transition to snow
	pl::Layer snow2 = pl::Layer(0.5, glm::vec3(1.0, 1.0, 1.0));        // Snow
#pragma endregion

	pl::Shader planetShader = pl::Shader("assets/shaders/noise.vert", "assets/shaders/noise.frag");
	pl::Shader waterShader = pl::Shader("assets/shaders/water.vert", "assets/shaders/water.frag");
	pl::Shader lightShader = pl::Shader("assets/shaders/light.vert", "assets/shaders/light.frag");
	pl::Shader spaceShader = pl::Shader("assets/shaders/space.vert", "assets/shaders/space.frag");
	pl::Texture2D container = pl::Texture2D("assets/textures/Texturelabs_Soil_134L.jpg", GL_LINEAR, GL_REPEAT);

    std::vector<std::string> faces;
    faces.push_back("assets/textures/skybox/right.png");
    faces.push_back("assets/textures/skybox/left.png");
    faces.push_back("assets/textures/skybox/bottom.png");
    faces.push_back("assets/textures/skybox/top.png");
    faces.push_back("assets/textures/skybox/front.png");
    faces.push_back("assets/textures/skybox/back.png");
    pl::Cubemap space = pl::Cubemap(faces);

	glm::mat4 planetTransform = glm::mat4(1.0f);
	//ptransform = glm::scale(ptransform, glm::vec3(planetTopology.minRadius));

	glm::mat4 waterTransform = glm::mat4(1.0f);
	//transform = glm::scale(transform, glm::vec3(planetTopology.waterLevel));

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

		processInput(window, &planetTransform, camera);
		processInput(window, &waterTransform, camera);

		// Draw
		// background
		glClearColor(0.2f, 0.2f, 0.5f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glEnable(GL_CULL_FACE);  

		// light box
		lightShader.use();
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, planetLight.pos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
		lightShader.setMat4("projection", projection);
		lightShader.setMat4("view", view);
		lightShader.setMat4("model", lightModel);
		
		lightShader.setVec3("lightPos", planetLight.pos);
		lightShader.setVec3("lightColor", planetLight.color);
		lightShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
		slight.Draw(lightShader);

		// planet
		glEnable(GL_DEPTH_TEST);
		planetShader.use();

		planetShader.setLight(planetLight);
		planetShader.setVec3("viewPos", camera.getPosition());
		planetShader.setMat4("projection", projection);
		planetShader.setMat4("view", view);
		planetShader.setMat4("model", planetTransform);

		planetShader.setFloat("minRadius", planetTopology.minRadius);
		planetShader.setFloat("maxRadius", planetTopology.maxRadius);
		planetShader.setFloat("mountainRoughness", planetTopology.mountainRoughness);
		planetShader.setFloat("waterLevel", planetTopology.waterLevel);
		planetShader.setFloat("frequency", planetTopology.mountainFrequency);
		planetShader.setVec3("seed", planetTopology.seed);

		planetShader.setLayer("waterDeep", waterDeep);
		planetShader.setLayer("waterShallow", waterShallow);
		planetShader.setLayer("sand", sand);
		planetShader.setLayer("land1", land1);
		planetShader.setLayer("land2", land2);
		planetShader.setLayer("land3", land3);
		planetShader.setLayer("snow1", snow1);
		planetShader.setLayer("snow2", snow2);

		container.Bind(GL_TEXTURE0);
		planet.Draw(planetShader);

		// water
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		waterShader.use();

		waterShader.setLight(waterLight);
		waterShader.setVec3("viewPos", camera.getPosition());
		waterShader.setMat4("projection", projection);
		waterShader.setMat4("view", view);
		waterShader.setMat4("model", waterTransform);

		waterShader.setVec3("waterColor", waterColor);
		waterShader.setFloat("waterLevel", planetTopology.waterLevel);
		waterShader.setFloat("waterAlpha", waterAlpha);

		waterShader.setFloat("waveAmplitude", waves.amplitude);
		waterShader.setFloat("waveFrequency", waves.frequency);
		waterShader.setFloat("waveSpeed", waves.speed);
		waterShader.setFloat("time", time);

		waterShader.setFloat("rimLightIntensity", waterLight.rimK);
		waterShader.setFloat("rimLightExponent", waterLight.rimFalloff);

		container.Bind(GL_TEXTURE0);
		water.Draw(planetShader);

		//space
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
glDepthMask(GL_FALSE);
		spaceShader.use();

		glm::mat4 skyboxProj = camera.perspectiveProjection();
		spaceShader.setMat4("projection", skyboxProj);
		spaceShader.setMat4("view", view = glm::mat4(glm::mat3(view)));
		spaceShader.setInt("skybox", 0);

		space.drawSkybox();
glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS); // set depth function back to default
        glEnable(GL_DEPTH_TEST);


#pragma region ImGui
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Lighting");
		ImGui::Checkbox("Blinn-Phong", &planetLight.blinnPhong);
		ImGui::DragFloat3("Light Position", &planetLight.pos.x, 0.1f);
		ImGui::ColorEdit3("Light Color", &planetLight.color.r);
		ImGui::SliderFloat("Ambient K", &planetLight.ambientK, 0.0f, 1.0f);
		ImGui::SliderFloat("Diffuse K", &planetLight.diffuseK, 0.0f, 1.0f);
		ImGui::SliderFloat("Specular K", &planetLight.specularK, 0.0f, 1.0f);
		ImGui::SliderFloat("Shininess", &planetLight.shininess, 2.0f, 1024.0f);
		ImGui::SliderFloat("Rim intensity", &planetLight.rimK, 0.0f, 5.0);
		ImGui::SliderFloat("Rim falloff", &planetLight.rimFalloff, 0.0f, 7.0f);
		ImGui::ColorEdit3("Rim color", &planetLight.rimColor.r);
		ImGui::End();

		ImGui::Begin("Planet");
		ImGui::SliderFloat("Ground Depth", &planetTopology.minRadius, 0.1f, 7.0f);
		ImGui::SliderFloat("Ground Peak", &planetTopology.maxRadius, 0.1f, 7.0f);
		ImGui::SliderFloat("Ground Roughness", &planetTopology.mountainRoughness, 0.05f, 7.0f);
		ImGui::SliderFloat("Water Level", &planetTopology.waterLevel, 0.05f, 7.0f);
		ImGui::SliderFloat("Mountain Frequency", &planetTopology.mountainFrequency, 0.05f, 15.0f);
		ImGui::DragFloat3("Seed", &planetTopology.seed.x, 0.1f);
		if (ImGui::Button("Randomize Seed"))
			planetTopology.randomizeSeed();
		ImGui::End();

		ImGui::Begin("Water");
		ImGui::Text("Water Color");
		ImGui::SliderFloat("Water alpha", &waterAlpha, 0.0, 1.0);
		ImGui::ColorEdit3("Water color", &waterColor.r);
		ImGui::ColorEdit3("Water rim light color", &waterLight.rimColor.r);
		ImGui::SliderFloat("Water rim light intensity", &waterLight.rimK, 0.0f, 2.0f);
		ImGui::SliderFloat("Water rim light falloff", &waterLight.rimFalloff, 0.0f, 7.0f);
		ImGui::Text("Waves");
		ImGui::SliderFloat("Waves amplitude", &waves.amplitude, 0.0f, 0.5f);
		ImGui::SliderFloat("Waves frequency", &waves.frequency, 0.0f, 30.0f);
		ImGui::SliderFloat("Waves speed", &waves.speed, 0.0f, 15.0f);
		ImGui::End();

		ImGui::Begin("Layers");
		ImGui::Text("Water Deep");
		ImGui::SliderFloat("Water deep altitude", &waterDeep.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Water deep color", &waterDeep.color.r);
		ImGui::Text("Water Shallow");
		ImGui::SliderFloat("Water shallow altitude", &waterShallow.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Water shallow color", &waterShallow.color.r);
		ImGui::Text("Sand");
		ImGui::SliderFloat("Sand altitude", &sand.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Sand color", &sand.color.r);
		ImGui::Text("Land 1");
		ImGui::SliderFloat("Land1 altitude", &land1.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Land1 color", &land1.color.r);
		ImGui::Text("Land 2");
		ImGui::SliderFloat("Land2 altitude", &land2.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Land2 color", &land2.color.r);
		ImGui::Text("Land 3");
		ImGui::SliderFloat("Land3 altitude", &land3.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Land3 color", &land3.color.r);
		ImGui::Text("Snow 1");
		ImGui::SliderFloat("Snow1 altitude", &snow1.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Snow1 color", &snow1.color.r);
		ImGui::Text("Snow 2");
		ImGui::SliderFloat("Snow 2 altitude", &snow2.altitude, -1.0f, 3.0f);
		ImGui::ColorEdit3("Snow 2 color", &snow2.color.r);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#pragma endregion

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

// Caleb Carreon
void processInput(GLFWwindow* window, glm::mat4* model, pl::Camera cam) {
	float rotationSpeed = 2.0f * deltaTime;
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotMatrix = glm::rotate(rotMatrix, rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotMatrix = glm::rotate(rotMatrix, rotationSpeed, glm::vec3(0.0f, -1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rotMatrix = glm::rotate(rotMatrix, rotationSpeed, glm::cross(glm::vec3(0.0f, -1.0f, 0.0f), cam.getPosition()));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rotMatrix = glm::rotate(rotMatrix, rotationSpeed, glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), cam.getPosition()));
	}
	*model = rotMatrix * (*model);
}

// Joseph Isaacs
glm::vec3 powColor(glm::vec3 color, float amount) {
	color.r = pow(color.r, amount);
	color.g = pow(color.g, amount);
	color.b = pow(color.b, amount);
	return color;
}

