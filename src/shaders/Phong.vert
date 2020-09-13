#version 330 core
// https://github.com/paulpela/lighting-opengl/tree/master/shaders

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_color;

uniform mat4 u_modelViewMatrix = mat4(1.0f);
uniform mat4 u_projectionMatrix = mat4(1.0f);
uniform mat4 u_modelViewProjectionMatrix = mat4(1.0f);

uniform vec3 u_lightDirection;

out VS_OUT {
    vec3 N;
    vec3 L;
    vec3 V;
} vs_out;

void main() {
    vec4 P = u_modelViewMatrix * vec4(v_position, 1.0);
    
    gl_Position = u_modelViewProjectionMatrix * vec4(v_position, 1.0);

    vs_out.N = mat3(u_modelViewMatrix) * v_normal;
    vs_out.L = u_lightDirection - P.xyz;
    vs_out.V = -P.xyz;
}
