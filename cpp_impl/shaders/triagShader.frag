#version 430 core
out vec4 FragColor;
uniform vec3 color;
in vec3 color_;
void main() {
    FragColor = vec4(color_, 1.0f);
}
