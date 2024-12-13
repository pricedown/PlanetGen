// Joseph Isaacs

#version 330 core

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  
in float Radius;

out vec4 FragColor;

uniform sampler2D tex;

// lighting
uniform vec3 viewPos;
uniform bool blinnPhong;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform float ambientStrength;
uniform float specularStrength;
uniform float diffuseStrength;
uniform float shininess;
uniform float rimStrength;
uniform float rimFalloff;
uniform vec3 rimColor;

// planet topology
uniform float minRadius;
uniform float maxRadius;
uniform float waterLevel;

struct Layer { // treated like a ColorStop
    float altitude; // relative to waterLevel
    vec3 color;
};
const int LAYER_COUNT = 8;
Layer layers[LAYER_COUNT];

uniform Layer waterDeep;
uniform Layer waterShallow;
uniform Layer sand;
uniform Layer land1;
uniform Layer land2;
uniform Layer land3;
uniform Layer snow1;
uniform Layer snow2;

vec3 powColor(vec3 color, float amount);
float normalizedToRadius(float normalizedAltitude);
float radiusToNormalized(float radius);

vec3 litColor(vec3 objectColor);
float grayScale(vec3 color);

vec3 altitudeColor(float altitude);

void main() {
  vec3 objectColor = vec3(texture(tex,TexCoord*5));

  vec3 altitudeCol;
  layers[0] = waterDeep;
  layers[1] = waterShallow;
  layers[2] = sand;
  layers[3] = land1;
  layers[4] = land2;
  layers[5] = land3;
  layers[6] = snow1;
  layers[7] = snow2;
  altitudeCol = altitudeColor(Radius-waterLevel); // makes values relative to waterLevel

  vec3 litColor = litColor(grayScale(objectColor) * altitudeCol);
  FragColor = vec4(litColor, 1.0);
}

vec3 altitudeColor(float altitude) {
  // clamp lowest
  if (layers[0].altitude > altitude) 
    return layers[0].color;

  // clamp highest
  if (layers[LAYER_COUNT-1].altitude < altitude)
    return layers[LAYER_COUNT-1].color;

  for (int i = 0; i < LAYER_COUNT-1; i++) {
    // if between layers
    if (layers[i].altitude < altitude && layers[i+1].altitude > altitude) {
      // interpolate color
      float t = (altitude - layers[i].altitude) 
              / (layers[i+1].altitude - layers[i].altitude);
      return mix(layers[i].color, layers[i+1].color, t);
    }
  }

  return vec3(0.0);
}

// uses either blinn-phong or phong
vec3 litColor(vec3 objectColor) {
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);  
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);  
  vec3 halfwayDir = normalize(lightDir + viewDir);

  vec3 ambient = ambientStrength * lightColor;

  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor * diffuseStrength;

  float spec = 0.0;
  if (blinnPhong)
    spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
  else
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = lightColor * spec * specularStrength;

  vec3 rim;
  float rimI = 1.0 - dot(viewDir, norm);
  rimI = pow(max(0.0, rimI), rimFalloff);
  rim = rimI * rimStrength * lightColor;

  return (rim + ambient + diffuse + specular) * objectColor;
}

float grayScale(vec3 color) {
  vec3 grayScale = vec3(0.2126, 0.7152, 0.0722);
  return dot(color,grayScale);
}

vec3 powColor(vec3 color, float amount) {
    color.r = pow(color.r, amount);
    color.g = pow(color.g, amount);
    color.b = pow(color.b, amount);
    return color;
}

float normalizedToRadius(float normalizedAltitude) {
    return minRadius + normalizedAltitude * abs(maxRadius - minRadius);
}

float radiusToNormalized(float radius) {
    return (radius - minRadius) / abs(maxRadius - minRadius);
}