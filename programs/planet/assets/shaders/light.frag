#version 330 core

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3 viewPos;

uniform vec3 lightColor;

void main() {
    FragColor = vec4(lightColor, 1.0);
}