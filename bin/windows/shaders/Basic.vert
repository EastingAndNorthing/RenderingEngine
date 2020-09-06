#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_mvp;

void main() { 
    gl_Position = position * u_mvp;
    // gl_Position = position;
}