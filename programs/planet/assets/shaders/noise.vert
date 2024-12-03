#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out float NormalizedAltitude;

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

float perlin_noise(vec2 uv, float cells_count)
{
    //Get pos in uv coord
    vec2 pos_in_grid = uv * cells_count;
    vec2 cell_pos_in_grid = floor(pos_in_grid);
    vec2 local_pos_in_cell = pos_in_grid - cell_pos_in_grid;

    //Blend
    vec2 blend = local_pos_in_cell * local_pos_in_cell * (3.0 - 2.0 * local_pos_in_cell);

    //Get the wraparound
    float grid_wrap = cells_count;
    vec2 wrapped_pos = mod(cell_pos_in_grid, grid_wrap);

    //Make it so the sides are the same as each other
    vec2 left_bottom = wrapped_pos;
    vec2 right_bottom = mod(wrapped_pos + vec2(1, 0), grid_wrap);
    vec2 left_top = mod(wrapped_pos + vec2(0, 1), grid_wrap);
    vec2 right_top = mod(wrapped_pos + vec2(1, 1), grid_wrap);

    float left_bottom_dot = dot(local_pos_in_cell - vec2(0.0, 0.0), get_gradient(left_bottom));
    float right_bottom_dot = dot(local_pos_in_cell - vec2(1.0, 0.0), get_gradient(right_bottom));
    float left_top_dot = dot(local_pos_in_cell - vec2(0.0, 1.0), get_gradient(left_top));
    float right_top_dot = dot(local_pos_in_cell - vec2(1.0, 1.0), get_gradient(right_top));

    float bottom_mix = mix(left_bottom_dot, right_bottom_dot, blend.x);
    float top_mix = mix(left_top_dot, right_top_dot, blend.x);
    float noise_value = mix(bottom_mix, top_mix, blend.y);

    return 0.5 + 0.5 * noise_value;
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



void main()
{
    vec2 uv = aTexCoord;

    vec3 col = 0.5 + 0.5*cos(uTime+uv.xyx+vec3(0,2,4));

    NormalizedAltitude = pow(Pseudo3dNoise(aPos * frequency), mountainRoughness);

    float mountainScalar = (maxRadius - minRadius)/minRadius;
    float altitude = mountainScalar * NormalizedAltitude;

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos + vec3(altitude) * aNormal, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}

//https://www.shadertoy.com/view/DsK3W1
//https://www.shadertoy.com/view/MtcGRl
