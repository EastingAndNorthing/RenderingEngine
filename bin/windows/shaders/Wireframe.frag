#version 330 core

uniform float uLineWidth = 1.0;
uniform vec4 uColor = vec4(0.0, 0, 0, 0);
// uniform float uBlendFactor; // 1.5..2.5
in vec2 vLineCenter;

out vec4 out_color;

void main(void) {

    vec4 col = uColor;
    float d = length(vLineCenter - gl_FragCoord.xy); // double?
    float w = uLineWidth;  // double?

    float uBlendFactor = 1.5;

    if (d > w)
        col.r = 0;
    else
        col.r *= pow(float((w-d)/w), uBlendFactor);

    // col.r = vLineCenter.x;

    out_color = col;
};
