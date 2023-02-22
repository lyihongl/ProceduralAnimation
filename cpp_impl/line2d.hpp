#pragma once
#include <shaders.hpp>
#include <vector>
#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "global.hpp"

class Line2DRender {
    unsigned int VBO, VAO;
    std::vector<float> vertices;
    // glm::vec2 startPoint;
    // glm::vec2 endPoint;
    glm::mat4 MVP;
    glm::vec3 lineColor;

    public:
    int shaderProgram;
    Line2DRender() {

        // startPoint = start;
        // endPoint = end;
        // start.x = 2 * start.x / (float)SCREEN_WIDTH - 1;
        // start.y = 2 * start.y / (float)SCREEN_HEIGHT - 1;
        // end.x = 2 * end.x / (float)SCREEN_WIDTH - 1;
        // end.y = 2 * end.y / (float)SCREEN_HEIGHT - 1;
        // startPoint = start;
        // endPoint = end;
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);
        /*

        float x1 = start.x;
        float y1 = start.y;
        float x2 = end.x;
        float y2 = end.y;
        float w = SCREEN_WIDTH;
        float h = SCREEN_HEIGHT;

        // convert 3d world space position 2d screen space position
        x1 = 2 * x1 / w - 1;
        y1 = 2 * y1 / h - 1;

        x2 = 2 * x2 / w - 1;
        y2 = 2 * y2 / h - 1;

        start.x = x1;
        start.y = y1;
        end.x = x2;
        end.y = y2;
        */

        // vertex shader
        // int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        // glCompileShader(vertexShader);
        // check for shader compile errors

        // fragment shader
        // int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        // glCompileShader(fragmentShader);
        // check for shader compile errors

        // link shaders
        // shaderProgram = glCreateProgram();
        // glAttachShader(shaderProgram, vertexShader);
        // glAttachShader(shaderProgram, fragmentShader);
        // glLinkProgram(shaderProgram);
        // check for linking errors

        // glDeleteShader(vertexShader);
        // glDeleteShader(fragmentShader);

        // vertices = {start.x, start.y, 0, end.x,     end.y,     0,
        //             end.x,   end.y,   0, end.x + 1, end.y + 1, 0};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
    }
    void clear() {
        vertices.clear();
    }

    void addLine(glm::vec2 start, glm::vec2 end) {
        // start.x = 2 * start.x / (float)SCREEN_WIDTH - 1;
        // start.y = 2 * start.y / (float)SCREEN_HEIGHT - 1;
        // end.x = 2 * end.x / (float)SCREEN_WIDTH - 1;
        // end.y = 2 * end.y / (float)SCREEN_HEIGHT - 1;
        vertices.push_back(start[0]);
        vertices.push_back(start[1]);
        vertices.push_back(end[0]);
        vertices.push_back(end[1]);
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
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1,
                           GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                     &lineColor[0]);
        glUniform1i(glGetUniformLocation(shaderProgram, "w"), SCREEN_WIDTH);
        glUniform1i(glGetUniformLocation(shaderProgram, "h"), SCREEN_HEIGHT);

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 2);
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

    ~Line2DRender() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};