#version 430 core

layout(location = 0) in vec2 aPos;
uniform mat4 MVP;
uniform int h, w;

void main() {
    gl_Position = MVP * vec4(2 * aPos.x / w - 1, 2 * aPos.y / h - 1, 0.0, 1.0);
    // gl_PointSize = 30.0;
}