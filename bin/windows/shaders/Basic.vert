#version 330 core

layout(location = 0) in vec3 position;

// uniform mat4 u_mvp;

void main() { 
    // gl_Position = u_mvp * vec4(position, 1.0);
    gl_Position = vec4(position, 1.0);
}