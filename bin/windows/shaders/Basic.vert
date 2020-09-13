#version 330 core

layout (location = 0) in vec3 v_position;

uniform mat4 u_modelViewProjectionMatrix = mat4(1.0f);

void main() { 
    gl_Position = u_modelViewProjectionMatrix * vec4(v_position, 1.0);
}