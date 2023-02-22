#pragma once
#include <shaders.hpp>
#include <vector>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "global.hpp"

class Point2DRender {
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    // glm::vec2 startPoint;
    // glm::vec2 endPoint;
    glm::mat4 MVP;
    glm::vec3 lineColor;

    public:
    int shaderProgram;
    Point2DRender() {
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    void clear() {
        vertices.clear();
    }

    void addPoint(glm::vec2 start) {
        start.x = 2 * start.x / (float)SCREEN_WIDTH - 1;
        start.y = 2 * start.y / (float)SCREEN_HEIGHT - 1;
        vertices.push_back(start[0]);
        vertices.push_back(start[1]);
        // vertices.push_back(end[0]);
        // vertices.push_back(end[1]);
    }
    void prepareDraw() {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
                     vertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    void drawExternal(std::vector<float> &v) {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(),
                     v.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)(2*sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(shaderProgram);
        glPointSize(5.f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1,
                           GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                     &lineColor[0]);
        glUniform1i(glGetUniformLocation(shaderProgram, "w"), SCREEN_WIDTH);
        glUniform1i(glGetUniformLocation(shaderProgram, "h"), SCREEN_HEIGHT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, v.size() / 3);
        // return 1;
    }

    int setMVP(glm::mat4 mvp) {
        MVP = mvp;
        return 1;
    }

    int setColor(glm::vec3 color) {
        lineColor = color;
        return 1;
    }

    int draw() {
        glUseProgram(shaderProgram);
        glPointSize(5.f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1,
                           GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                     &lineColor[0]);

        glBindVertexArray(VAO);
        glDrawArrays(GL_POINTS, 0, vertices.size() / 2);
        return 1;
    }
    void debugPrint() {
        for (int i = 0; i < vertices.size(); i += 4) {
            std::cout << "line start(" << vertices[i] << "," << vertices[i + 1]
                      << ") end(" << vertices[i + 2] << "," << vertices[i + 3]
                      << ")" << std::endl;
        }
    }

    // int draw2D(int w, int h) {
    //     glUseProgram(shaderProgram);
    //     glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1,
    //                        GL_FALSE, &MVP[0][0]);
    //     glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
    //                  &lineColor[0]);

    //     glBindVertexArray(VAO);
    //     glDrawArrays(GL_LINES, 0, 2);
    //     return 1;

    // }

    ~Point2DRender() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};