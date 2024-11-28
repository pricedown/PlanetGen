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
    vec2 pos_in_grid = uv * cells_count;
    vec2 cell_pos_in_grid =  floor(pos_in_grid);
    vec2 local_pos_in_cell = (pos_in_grid - cell_pos_in_grid);
    vec2 blend = local_pos_in_cell * local_pos_in_cell * (3.0f - 2.0f * local_pos_in_cell);
    
    vec2 left_top = cell_pos_in_grid + vec2(0, 1);
    vec2 right_top = cell_pos_in_grid + vec2(1, 1);
    vec2 left_bottom = cell_pos_in_grid + vec2(0, 0);
    vec2 right_bottom = cell_pos_in_grid + vec2(1, 0);
    
    float left_top_dot = dot(pos_in_grid - left_top, get_gradient(left_top));
    float right_top_dot = dot(pos_in_grid - right_top,  get_gradient(right_top));
    float left_bottom_dot = dot(pos_in_grid - left_bottom, get_gradient(left_bottom));
    float right_bottom_dot = dot(pos_in_grid - right_bottom, get_gradient(right_bottom));
    
    float noise_value = mix(
                            mix(left_bottom_dot, right_bottom_dot, blend.x), 
                            mix(left_top_dot, right_top_dot, blend.x), 
                            blend.y);
   
    
    return (0.5 + 0.5 * (noise_value / 0.7));
}

void main()
{
    vec2 uv = aTexCoord;

    vec3 col = 0.5 + 0.5*cos(uTime+uv.xyx+vec3(0,2,4));

    NormalizedAltitude = pow(perlin_noise(uv,10.0), mountainRoughness);

    float mountainScalar = (maxRadius - minRadius)/minRadius;
    float height = mountainScalar * NormalizedAltitude;

    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos + vec3(height) * aNormal, 1.0);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));
}

//https://www.shadertoy.com/view/DsK3W1
