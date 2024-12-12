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
uniform float rimLightIntensity;
uniform float rimLightShininess;

// water
uniform float waterAlpha;
uniform vec3 waterColor;

void main() {
    vec3 objectColor = vec3(texture(tex,TexCoord));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    vec3 halfwayDir = normalize(lightDir + viewDir);

    vec3 ambient = ambientStrength * lightColor;

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * diffuseStrength * 0.9;

    float spec = 0.0;
    if (blinnPhong)
    {
        spec = pow(max(dot(norm, halfwayDir), 0.0), shininess*1.3);
    }
    else
    {
        spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess*1.3);
    }
    vec3 specular = waterColor * lightColor * spec * specularStrength*1.4f;


    vec3 rim;
    float rimF = dot(viewDir, norm);
    rimF = pow(max(0.0, 1.0 - rimF), rimLightShininess);
    rim = rimF * rimLightIntensity * lightColor;

    vec3 result = (rim + ambient + diffuse + specular) * waterColor;


    FragColor = vec4(result, waterAlpha);
}
