// Joseph Isaacs
#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    /*
    TexCoord = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0);
    */
    /*
    TexCoord = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww; // maximum depth value, ensuring it will only be rendered wherever there are no objects visible
                        */

        vec3 sphericalPosition = normalize(aPos);

    TexCoord = sphericalPosition;
    
    // Apply the view and projection transformations
    vec4 pos = projection * view * vec4(sphericalPosition, 1.0);
    
    // Set the final position for rendering
    gl_Position = pos.xyww;  // This ensures the maximum depth value for skybox rendering
}
