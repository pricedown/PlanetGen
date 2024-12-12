#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec4 vColor;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float waterLevel;
uniform float time;
uniform float waveAmplitude;
uniform float waveFrequency;
uniform float waveSpeed;

void main() {
	float height = waterLevel;
	float waveHeight = 0.0;
	// randomly overlapping waves
    waveHeight += waveAmplitude * sin((waveFrequency * (aPos.x + aPos.y) + waveSpeed * time));
    waveHeight += 0.5 * waveAmplitude * sin((2.0 * waveFrequency * (aPos.x + aPos.z) + waveSpeed * time));
    waveHeight += 0.3 * waveAmplitude * sin((0.5 * waveFrequency * (aPos.x - aPos.z) + waveSpeed * time));
    height += waveHeight;
	height += waveHeight;

	vec3 pos = height * aNormal;
	gl_Position = projection * view * model * vec4(pos,1.0);
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(model))) * aNormal;  
	FragPos = vec3(model * vec4(pos, 1.0));
}
