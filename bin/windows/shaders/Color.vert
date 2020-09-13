#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

uniform mat4 u_modelViewProjectionMatrix = mat4(1.0f);

void main() { 
    gl_Position = u_modelViewProjectionMatrix * vec4(position, 1.0);
}
