#version 430 core
uniform vec3 color;

out vec4 FragColor;
in float c_;
// in vec2 position;
// in vec2 center;

void main() {
    // float R = 0.00;

    // if (length(position - center) > R) {
    //     discard;
    // }
    FragColor = vec4(c_, 1, 0, 1);
}
