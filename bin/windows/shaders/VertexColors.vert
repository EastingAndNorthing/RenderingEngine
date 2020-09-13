#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 color;

out vec4 vertexColor;

uniform mat4 u_mvp;

void main() { 
    gl_Position = u_mvp * vec4(position, 1.0);
    vertexColor = color;
}
