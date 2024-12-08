#version 330 core

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  
in float NormalizedAltitude;

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

// planet
uniform float minRadius;
uniform float maxRadius;
uniform float waterLevel;

// Treated as a ColorStop
// https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_images/Using_CSS_gradients
struct Layer {
    float altitude; // normalized
    vec3 color;
};

const int LAYER_COUNT = 8;
Layer layers[LAYER_COUNT];

vec3 grayScale = vec3(0.2126, 0.7152, 0.0722);

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

vec3 powColor(vec3 color, float amount) {
    color.r = pow(color.r, amount);
    color.g = pow(color.g, amount);
    color.b = pow(color.b, amount);
    return color;
}

void main() {
  vec3 objectColor = vec3(texture(tex,TexCoord));

  float objectColorGrayscale = dot(objectColor, grayScale);

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
  {
    spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
  }
  else
  {
    spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  }
  vec3 specular = lightColor * spec * specularStrength;

  vec3 result = (ambient + diffuse + specular) * objectColorGrayscale;
  vec3 green = vec3(0.0, 1.0, 0.0);
  vec3 blue = vec3(0.0, 0.0, 1.0);

  float normalizedWaterLevel = (waterLevel-minRadius) / (maxRadius-minRadius);

  float waterDeep = -0.4;
  vec3 waterDeepest = vec3(0.05,0.05,1.0);
  vec3 waterLand = vec3(0.2,0.2,0.3);
  vec3 sand = vec3(0.79, 0.74, 0.57);
  float waterTransition = 0.1;
  float sandTransition = 0.1;

  vec3 altitudeCol;
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

  layers[0] = Layer(waterDeep, waterDeepest);       // Water below sea
  layers[1] = Layer(normalizedWaterLevel-waterTransition, waterLand);        // Water at sea level
  layers[2] = Layer(normalizedWaterLevel, sand);        // Water at sea level
  layers[3] = Layer(normalizedWaterLevel+sandTransition, powColor(vec3(0.333, 0.419, 0.184), 0.005)*sand*vec3(0.133, 0.530, 0.133));  // Land
  layers[4] = Layer(0.75, vec3(0.333, 0.419, 0.184));  // Land (more olive green)
  layers[5] = Layer(0.8, vec3(0.345, 0.471, 0.074));  // Land (brownish)
  layers[6] = Layer(0.85, vec3(0.933, 0.933, 0.933)); // Transition to snow
  layers[7] = Layer(1.0, vec3(1.0, 1.0, 1.0));        // Snow 
  altitudeCol = altitudeColor(NormalizedAltitude);

  FragColor = vec4(altitudeCol * result, 1.0);

  // Debugs
  //FragColor = vec4(result, 1.0);
  //FragColor = vec4(Normal,1.0);
}
