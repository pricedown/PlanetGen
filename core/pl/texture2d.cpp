#include "texture2d.h"

using namespace pl;

Texture2D::Texture2D(const char* filePath, int filterMode, int wrapMode) {
	stbi_set_flip_vertically_on_load(1);
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);
	// load and generate the texture
	int nrChannels;
	unsigned char* data = stbi_load(filePath, &m_width, &m_height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}
Texture2D::Texture2D(float width, float height, int persistence, int numOctaves) {
	m_width = width;
	m_height = height;
	
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glTextureStorage2D(m_id, 1, GL_RGB32F, m_width, m_height);
	glBindTexture(GL_TEXTURE_2D, m_id);

	float* data = new float[m_width * m_height * 3];
	Noise noise = Noise(persistence, numOctaves);
	for (int i = 0; i < m_width * m_height; i += 3) {
		data[i] = noise.interpolateNoise(i % m_height, i / m_height) / 500.0;
		data[i + 1] = data[i];
		data[i + 2] = data[i];
		//std::cout << data[i] << std::endl;
	}
	glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, data);
	delete[] data;
}
Texture2D::~Texture2D() {
	glDeleteTextures(1, &m_id);
}

void Texture2D::Bind(unsigned int slot) {
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}