#version 330 core

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  

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

// water
uniform float waterAlpha;
uniform vec3 waterColor;

vec3 litColor(vec3 objectColor);

void main() {
    vec3 objectColor = vec3(texture(tex,TexCoord));

    vec3 litColor = litColor(objectColor);

    FragColor = vec4(litColor, waterAlpha);
}

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
    {
        spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    }
    else
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    }
    vec3 specular = waterColor * lightColor * spec * specularStrength;

    return (ambient + diffuse + specular) * waterColor;
}
