#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out vec4 vertexColor;

uniform mat4 u_mvp;

void main() { 
    gl_Position = position * u_mvp;
    vertexColor = color;
}
