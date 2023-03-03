#version 430 core

layout(location = 0) in vec2 aPos;
uniform mat4 MVP;
uniform int h, w;

// flat out vec3 color_;
out vec3 color_;
void main() {
    // color_ = vec3(2*(aPos.x - mod(aPos.x, 20.f))/w - 1, 2*(aPos.y - mod(aPos.y, 20.f))/h - 1, 0);
    // color_ = vec3(2*(int(aPos.x/2)*2)/w-1, 2*(int(aPos.y/2)*2)/h - 1, 0);
    gl_Position = MVP * vec4(2 * aPos.x / w - 1, 2 * aPos.y / h - 1, 0.0, 1.0);
    // color_ = vec3(gl_Position.xy, 0);
    color_ = vec3(sqrt(pow(gl_Position.x - mod(gl_Position.x, 0.05), 2)), gl_Position.y - mod(gl_Position.y, 0.05), (gl_Position.x + gl_Position.y)/2 - mod((gl_Position.x + gl_Position.y)/2, 0.05));
    // gl_PointSize = 30.0;
}