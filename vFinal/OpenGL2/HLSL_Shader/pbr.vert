#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;


out vec2 TexCoords;//texCoord
// Outputs the current position for the Fragment Shader
out vec3 WorldPos;//crntPos
out vec3 Normal;

//cameraMatrix = projection * view;
uniform mat4 camMatrix;
//uniform mat4 projection;
//uniform mat4 view;
uniform mat4 model;

uniform mat4 translation; //you
uniform mat4 rotation;//you
uniform mat4 scale;//you


void main()
{
    //·­×ªµÄvertices
    TexCoords =  mat2(0.0, -1.0, 1.0, 0.0) * aTexCoords;
    //WorldPos = vec3(model * vec4(aPos, 1.0)); 
    //crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0f));
    WorldPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.0));
    Normal =  mat3(model) * aNormal;   
   // gl_Position =  projection * view * vec4(WorldPos, 1.0);
    gl_Position =  camMatrix * vec4(WorldPos, 1.0);
}