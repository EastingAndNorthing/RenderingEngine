#version 330 core
//https://vitaliburkov.wordpress.com/2016/09/17/simple-and-fast-high-quality-antialiased-lines-with-opengl/

layout (location = 0) in vec3 v_position;

uniform mat4 u_modelViewProjectionMatrix = mat4(1.0f);
uniform vec2 uViewPort = vec2(2560, 1440); // 1280x720 * 200% scaling #hardcoded;

out vec2 vLineCenter;

void main(void) {
    vec4 pp = u_modelViewProjectionMatrix * vec4(v_position, 1.0);
    gl_Position = pp;

    vec2 vp = uViewPort;
    vLineCenter = 0.5 * (pp.xy + vec2(1, 1)) * vp;
};
