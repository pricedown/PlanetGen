#pragma once

#ifndef SHADER_H
#define SHADER_H

#include "../ew/external/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../pl/lighting.h"
#include "../pl/planet.h"

namespace pl {
	class Shader {
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, float v1, float v2, float v3) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
		void setLight(const pl::Light& light) const;
		void setLayer(const std::string& name, const pl::Layer& layer) const;
	};
	#endif
}
