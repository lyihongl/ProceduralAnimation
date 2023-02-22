#version 430 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in float c;
uniform mat4 MVP;
uniform int h, w;

// out vec2 position;
// out vec2 center;
out float c_;

// out vec2 center;

void main() {
    gl_Position = MVP * vec4(2*aPos.x/w - 1, 2*aPos.y/h - 1, 0.0, 1.0);
    // center = vec2(2*(aPos.x-2.5)/w-1, 2*(aPos.y-2.5)/h-1);
    // position = gl_Position.xy;
    c_ = c;
    // gl_PointSize = 30.0;
}