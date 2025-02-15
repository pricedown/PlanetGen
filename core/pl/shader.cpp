#include "shader.h"

namespace pl {
	Shader::Shader(const char* vertexPath, const char* fragmentPath) {
		// Get vertex and fragment shader code from fields
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Allow objects to throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file buffers
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		// Compile shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// Vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		// Return any errors
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// Fragment shaders
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);

		glCompileShader(fragment);
		// Return any errors
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// Create shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		// Return any errors
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		// Delete redundant shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::use() {
		glUseProgram(ID);
	}

	void Shader::setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void Shader::setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void Shader::setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void Shader::setVec3(const std::string& name, float v1, float v2, float v3) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
	}
	void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void Shader::setLight(const pl::Light& light) const {
		setVec3("lightPos", light.pos);
		setVec3("lightColor", light.color);
		setVec3("rimColor", light.rimColor);
		setFloat("ambientStrength", light.ambientK);
		setFloat("specularStrength", light.specularK);
		setFloat("diffuseStrength", light.diffuseK);
		setFloat("rimStrength", light.rimK);
		setFloat("rimFalloff", light.rimFalloff);
		setFloat("shininess", light.shininess);
		setBool("blinnPhong", light.blinnPhong);
	}
	void Shader::setLayer(const std::string& name, const pl::Layer& layer) const {
		setFloat(name + ".altitude", layer.altitude);
		setVec3(name + ".color", layer.color);
	}
} // namespace pl
