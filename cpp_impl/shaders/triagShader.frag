#version 430 core
out vec4 FragColor;
uniform vec3 color;
flat in vec3 color_;
// in vec3 color_;
void main() {
    // FragColor= vec4(sqrt(pow(color_.x - mod(color_.x, 0.07), 2)), sqrt(pow(color_.y - mod(color_.y, 0.07), 2)), color_.z - mod(color_.z, 0.07), 1.0f);
    FragColor = vec4(color_, 1.0f);
}
