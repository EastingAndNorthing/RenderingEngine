#version 330 core

in vec4 position;
in vec4 color;

out vec4 colorV;

void main() { 
    gl_Position = position;
    colorV = color;
}