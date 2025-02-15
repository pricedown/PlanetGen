// Caleb Carreon

#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out float Radius;

uniform float uTime;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// planet
uniform float minRadius;
uniform float maxRadius;
uniform float mountainRoughness;

// noise 
uniform float frequency;
uniform vec3 seed;

float Pseudo3dNoise(vec3 pos);

float normalizedToRadius(float normalizedAltitude);
float radiusToNormalized(float radius);

void main()
{
    vec2 uv = aTexCoord;
    vec3 col = 0.5 + 0.5*cos(uTime+uv.xyx+vec3(0,2,4));

    float mountainHeight = maxRadius - minRadius;
    float noise = Pseudo3dNoise((aPos + seed) * frequency);
    float normalizedHills = pow(noise, mountainRoughness);
    float hills = normalizedHills * mountainHeight;
    
    Radius = minRadius + hills;
    vec3 pos = (Radius * aNormal);

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(pos, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(pos, 1.0));
}

vec2 n22 (vec2 p)
{
    vec3 a = fract(p.xyx * vec3(123.34, 234.34, 345.65));
    a += dot(a, a + 34.45);
    return fract(vec2(a.x * a.y, a.y * a.z));
}

vec2 get_gradient(vec2 pos)
{
    float twoPi = 6.283185;
    float angle = n22(pos).x * twoPi;
    return vec2(cos(angle), sin(angle));
}

vec2 GetGradient(vec2 intPos, float t) {
    
    // Uncomment for calculated rand
    float rand = fract(sin(dot(intPos, vec2(12.9898, 78.233))) * 43758.5453);;
    
    // Texture-based rand (a bit faster on my GPU)
    //float rand = perlin_noise(intPos / 64.0, 1000000.0);
    
    // Rotate gradient: random starting rotation, random rotation rate
    float angle = 6.283185 * rand + 4.0 * t * rand;
    return vec2(cos(angle), sin(angle));
}

float Pseudo3dNoise(vec3 pos) {
    vec2 i = floor(pos.xy);
    vec2 f = pos.xy - i;
    vec2 blend = f * f * (3.0 - 2.0 * f);
    float noiseVal = 
        mix(
            mix(
                dot(GetGradient(i + vec2(0, 0), pos.z), f - vec2(0, 0)),
                dot(GetGradient(i + vec2(1, 0), pos.z), f - vec2(1, 0)),
                blend.x),
            mix(
                dot(GetGradient(i + vec2(0, 1), pos.z), f - vec2(0, 1)),
                dot(GetGradient(i + vec2(1, 1), pos.z), f - vec2(1, 1)),
                blend.x),
        blend.y
    );
    return 0.5 + 0.5 * (noiseVal / 0.7); // normalize to about [-1..1]
}

float normalizedToRadius(float normalizedAltitude) {
    return minRadius + normalizedAltitude * abs(maxRadius - minRadius);
}

float radiusToNormalized(float radius) {
    return (radius - minRadius) / abs(maxRadius - minRadius);
}

//https://www.shadertoy.com/view/DsK3W1
//https://www.shadertoy.com/view/MtcGRl
