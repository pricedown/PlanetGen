// Joseph Isaacs

#version 330 core

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  
in float NormalizedAltitude;
in float Radius;

out vec4 FragColor;

uniform sampler2D tex;

uniform vec3 viewPos;

// lighting
uniform bool blinnPhong;
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform float ambientStrength;
uniform float specularStrength;
uniform float diffuseStrength;
uniform float shininess;

// planet topology
uniform float minRadius;
uniform float maxRadius;
uniform float waterLevel;

// Treated as a ColorStop
// https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_images/Using_CSS_gradients
struct Layer {
    float altitude; // normalized [0-1] from lowest depth to highest peak
    vec3 color;
};

  uniform Layer waterDeep;
  uniform Layer waterShallow;
  uniform Layer sand;
  uniform Layer land1;
  uniform Layer land2;
  uniform Layer land3;
  uniform Layer snow1;
  uniform Layer snow2;


const int LAYER_COUNT = 8;
Layer layers[LAYER_COUNT];
vec3 altitudeColor(float altitude);

vec3 powColor(vec3 color, float amount);

float normalizedToRadius(float normalizedAltitude) {
    return minRadius + normalizedAltitude * abs(maxRadius - minRadius);
}

float radiusToNormalized(float radius) {
    return (radius - minRadius) / abs(maxRadius - minRadius);
}

vec3 litColor(vec3 objectColor);
vec3 grayScale(vec3 color);

void main() {
  vec3 objectColor = vec3(texture(tex,TexCoord));
  vec3 grayColor = grayScale(objectColor);

  vec3 altitudeCol;


  float nWaterLevel = radiusToNormalized(waterLevel);

  /*
  float waterDeep_level = waterLevel - 0.9;
  float waterShallow_level = waterLevel - 0.1;
  float sand_level = waterLevel;
  float land1_level = waterLevel + 0.1;
  float land2_level = waterLevel + 0.25;
  float land3_level = waterLevel + 0.3;
  float snow1_level = waterLevel + 0.35;
  float snow2_level = waterLevel + 0.5;

  Layer waterDeep = Layer(waterDeep_level, vec3(0.05,0.05,1.0));
  Layer waterShallow = Layer(waterShallow_level, vec3(0.2,0.2,0.3));
  Layer sand = Layer(sand_level, vec3(0.79,0.74,0.57));
  Layer land1 = Layer(land1_level, powColor(vec3(0.333, 0.419, 0.184), 0.005)*vec3(0.79,0.74,0.57)*vec3(0.133, 0.530, 0.133));
  Layer land2 = Layer(land2_level, vec3(0.333, 0.419, 0.184));   // Land (more olive green)
  Layer land3 = Layer(land3_level, vec3(0.345, 0.471, 0.074));  // Land (brownish)
  Layer snow1 = Layer(snow1_level, vec3(0.933, 0.933, 0.933)); // Transition to snow
  Layer snow2 = Layer(snow2_level, vec3(1.0, 1.0, 1.0));        // Snow
  */

  layers[0] = waterDeep;
  layers[1] = waterShallow;
  layers[2] = sand;
  layers[3] = land1;
  layers[4] = land2;
  layers[5] = land3;
  layers[6] = snow1;
  layers[7] = snow2;

  altitudeCol = altitudeColor(Radius+2-waterLevel);

  vec3 litColor = litColor(grayColor*altitudeCol);

  FragColor = vec4(litColor, 1.0);

  // Debugs
  //FragColor = vec4(result, 1.0);
  //FragColor = vec4(Normal,1.0);
}

vec3 altitudeColor(float altitude) {
  if (layers[0].altitude > altitude)
    return layers[0].color;

  if (layers[LAYER_COUNT-1].altitude < altitude)
    return layers[LAYER_COUNT-1].color;

  for (int i = 0; i < LAYER_COUNT-1; i++) {
    // check if between layers
    if (layers[i].altitude < altitude && layers[i+1].altitude > altitude) {
      // blend between layers
      float t = (altitude - layers[i].altitude) / (layers[i + 1].altitude - layers[i].altitude);
      return mix(layers[i].color, layers[i+1].color, t);
    }
  }

  return vec3(0.0);
}

// Second layer layout
  /*
  layers[0] = Layer(waterDeep, waterDeepest);       // Water below sea
  layers[1] = Layer(normalizedWaterLevel-waterTransition, waterLand);        // Water at sea level
  layers[2] = Layer(normalizedWaterLevel, sand);        // Water at sea level
  layers[3] = Layer(normalizedWaterLevel+sandTransition, powColor(vec3(0.333, 0.419, 0.184), 0.005)*sand*vec3(0.133, 0.530, 0.133));  // Land
  layers[4] = Layer(0.75, vec3(0.333, 0.419, 0.184));  // Land (more olive green)
  layers[5] = Layer(0.8, vec3(0.345, 0.471, 0.074));  // Land (brownish)
  layers[6] = Layer(0.85, vec3(0.933, 0.933, 0.933)); // Transition to snow
  layers[7] = Layer(1.0, vec3(1.0, 1.0, 1.0));        // Snow 
  */


// First layer layout
  /*
  vec3 sand = vec3(0.79, 0.74, 0.57);
  vec3 land = vec3(0.133, 0.545, 0.133);
  vec3 altitudeCol;
  layers[0] = Layer(-10.0, vec3(0.0, 0.0, 1.0));       // Water below sea
  layers[1] = Layer(normalizedWaterLevel-0.1, vec3(0.0,0.0,0.2f));        // Water at sea level
  layers[2] = Layer(normalizedWaterLevel, sand);  // Land
  layers[3] = Layer(normalizedWaterLevel+0.1, land);  // Land
  layers[4] = Layer(0.7, vec3(0.333, 0.419, 0.184));  // Land (more olive green)
  layers[5] = Layer(0.8, vec3(0.345, 0.471, 0.074));  // Land (brownish)
  layers[6] = Layer(0.85, vec3(0.933, 0.933, 0.933)); // Transition to snow
  layers[7] = Layer(1.0, vec3(1.0, 1.0, 1.0));        // Snow 
  altitudeCol = altitudeColor(NormalizedAltitude);
  */

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

  vec3 result = (ambient + diffuse + specular) * objectColor;
  return result;
}

vec3 grayScale(vec3 color) {
  vec3 grayScale = vec3(0.2126, 0.7152, 0.0722);
  return color * dot(color,grayScale);
}

vec3 powColor(vec3 color, float amount) {
    color.r = pow(color.r, amount);
    color.g = pow(color.g, amount);
    color.b = pow(color.b, amount);
    return color;
}
