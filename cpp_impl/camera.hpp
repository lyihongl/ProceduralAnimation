#pragma once
#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 position;
    Camera() {
        position = glm::vec3(0,0,0);
    }
};