#version 430 core

layout(location = 0) in vec2 aPos;
uniform mat4 MVP;
uniform int h, w;

out vec3 color_;
void main() {
    gl_Position = MVP * vec4(2 * aPos.x / w - 1, 2 * aPos.y / h - 1, 0.0, 1.0);
    color_ = vec3(gl_Position.x, gl_Position.y, (gl_Position.x + gl_Position.y)/2);
    // gl_PointSize = 30.0;
}