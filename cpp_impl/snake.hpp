#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "line2d.hpp"
// #include <tuple>

class Snake {
    public:
    glm::vec2 start;
    std::unordered_map<std::size_t, std::vector<std::pair<int, float>>>
        connections;
    std::unordered_set<std::size_t> lock;
    std::vector<glm::vec2> points;
    Snake(glm::vec2 start);
    std::size_t addPoint();
    void addConnection(std::size_t a, std::size_t b, float d);
    void initPlacement();
    void update();
    void moveStart(glm::vec2 p);
};

Snake::Snake(glm::vec2 start) : start(start) {
}

std::size_t Snake::addPoint() {
    points.push_back(start);
    return points.size() - 1;
}

void Snake::addConnection(std::size_t a, std::size_t b, float d) {
    if (a == b) {
        throw -1;
    }
    connections[a].push_back({b, d});
    connections[b].push_back({a, d});
}

// float dist(glm::vec2)
void Snake::initPlacement() {
    std::unordered_set<std::size_t> placed;
    for (std::size_t i = 0; i < points.size(); i++) {
        if (connections.find(i) == connections.end())
            continue;                                 // has no connections
        if (placed.find(i) != placed.end()) continue; // already placed
        if (i == 0) {
            placed.insert(i);
            continue;
        }

        for (auto &[b, d] : connections[i]) {
            std::cout << "i: " << i << " b:" << b << " abs:"
                      << (abs(glm::length(points[b] - points[i]) - d))
                      << " find:" << (placed.find(b) != placed.end())
                      << std::endl;
            // std::cout <<"i: "<<i<<" "<<(abs(glm::length(points[b] -
            // points[i]) - d))<<std::endl;
            for (auto &p : placed) {
                std::cout << p << std::endl;
            }
            if (abs(glm::length(points[b] - points[i]) - d) <= 0.001) continue;
            // std::cout <<"A "<<b<<" "<<(placed.find(b) !=
            // placed.end())<<std::endl;
            if (placed.find(b) != placed.end()) {
                std::cout << "placing " << i << std::endl;
                placed.insert(i);
                points[i] = glm::vec2(points[i][0], points[i][1] + d);
            }
        }
    }
}

void Snake::update() {
    for (std::size_t i = 0; i < points.size(); i++) {
        if (lock.find(i) != lock.end()) continue;
        auto &point = points[i];
        lock.insert(i);
        if (connections[i].size() == 0) continue;

        std::size_t other = 0;
        std::pair<float, std::size_t> top_dist = {0.f, other};

        for (std::size_t ii = 0; ii < connections[i].size(); ii++) {
            auto &p = connections[i][ii];
            float dist = glm::length(points[p.first] - point);
            if (dist > top_dist.first) {
                top_dist = {dist, ii};
            }
        }
        other = top_dist.second;
        if (top_dist.first < connections[i][other].second) continue;
        glm::vec2 direction = points[connections[i][other].first] - point;
        float angle = std::atan2(direction[1], direction[0]);
        float adj_dist = connections[i][other].second;
        float y = glm::sin(angle) * adj_dist;
        float x = glm::cos(angle) * adj_dist;
        points[i] = points[connections[i][other].first] - glm::vec2(x, y);
    }
}

void Snake::moveStart(glm::vec2 p) {
    lock.insert(0);
    points[0] = p;
    update();
    lock.clear();
}

class SnakeRender2D {
    unsigned int VBO, VAO;
    glm::mat4 MVP;
    glm::vec3 lineColor;

    public:
    int shaderProgram;
    Line2DRender &line;
    std::vector<float> vertices;
    SnakeRender2D(Line2DRender &line) : line(line) {
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
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

        glBindVertexArray(VAO);
        glDrawArrays(GL_LINES, 0, vertices.size() / 2);
        return 1;
    }

    void computePoints(Snake &s) {
        vertices.clear();
        for (int i = 0; i < s.points.size(); i++) {
            if (s.connections[i].size() != 2) continue;
            // auto &c = s.connections[i];
            auto &p_c = s.points[i];
            std::vector<glm::vec2> p;
            // bool front = true;
            glm::vec2 forward;
            for (auto &otherT : s.connections[i]) {
                // front = otherT.first - i > 0;
                if (otherT.first < i) {
                    forward = s.points[otherT.first] - p_c;
                }
                p.push_back({s.points[otherT.first]});
            }
            glm::vec2 l1 = p_c - p[0];
            glm::vec2 l2 = p[1] - p_c;

            float a1 = std::atan2(l1[1], l1[0]);
            float a2 = std::atan2(l2[1], l2[0]);
            float diffa = fabs((glm::pi<float>() - fabs(a2 - a1)) / 2);
            // if(!(diffa <= ))
            // if (diffa < 0) {
            //     diffa += glm::pi<float>();
            // }

            // auto a90v = std::atan2()
            // float p1 =
            float polar1 =
                std::atan2(forward[1], forward[0]) + 2 * glm::pi<float>();
            // if (i == 1)
            //     std::cout << "forward: " << polar1 << " diffa: " << diffa
            //               << std::endl;

            diffa += polar1;
            glm::vec2 np1 =
                p_c + 20.f * glm::vec2(glm::cos(diffa), glm::sin(diffa));
            glm::vec2 np2 =
                p_c - 20.f * glm::vec2(glm::cos(diffa), glm::sin(diffa));
            // if (abs(diffa - front ? a1 : a2) <= glm::pi<float>() / 2) {
            // if (diffa - polar1 <= glm::pi<float>() / 2) {
            vertices.push_back(np1[0]);
            vertices.push_back(np1[1]);
            vertices.push_back(0.f);
            vertices.push_back(np2[0]);
            vertices.push_back(np2[1]);
            vertices.push_back(1.f);
            // } else {
            //     vertices.push_back(np2[0]);
            //     vertices.push_back(np2[1]);
            //     vertices.push_back(1.f);
            //     vertices.push_back(np1[0]);
            //     vertices.push_back(np1[1]);
            //     vertices.push_back(0.f);
            // }
            // } else {
            // }

            // glm::vec2 slope = np1 - np2;

            // return vertices;
        }
        // for(int i = 0; i<line.)
    }
    // void setShader(unsigned int shader) {
    //     // line.shaderProgram = shader;
    //     // shaderProgram = shader;
    // }
    ~SnakeRender2D() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);
    }
};

class SnakeMesh2D {
    unsigned int VBO, VAO, EBO;
    glm::mat4 MVP;
    glm::vec3 lineColor;
    // std::size_t eboSize;

    public:
    int shaderProgram;
    // Line2DRender &line;
    // std::vector<float> vertices;
    // std::vector<Uint32> eboIndices;
    SnakeMesh2D() {
        lineColor = glm::vec3(1, 1, 1);
        MVP = glm::mat4(1.0f);
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // for (int i = 0; i < s.points.size() - 2; i += 2) {
        //     /*
        //         0 1
        //         2 3
        //         4 5
        //     */
        //     eboIndices.push_back(i);
        //     eboIndices.push_back(i + 1);
        //     eboIndices.push_back(i + 2);
        //     eboIndices.push_back(i + 1);
        //     eboIndices.push_back(i + 2);
        //     eboIndices.push_back(i + 3);
        // }
        // glBindVertexArray(VAO);
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        // sizeof(Uint32)*eboIndices.size(), eboIndices.data(),
        //              GL_STATIC_DRAW);
        // // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }
    void draw(std::vector<float> &v) {
        glBindVertexArray(VAO);
        std::vector<Uint32> eboInd;
        int j = 0;
        for (int i = 0; i < v.size(); i += 6) {
            eboInd.push_back(j);
            eboInd.push_back(j + 1);
            eboInd.push_back(j + 2);
            eboInd.push_back(j + 1);
            eboInd.push_back(j + 2);
            eboInd.push_back(j + 3);
            j += 2;
        }

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), v.data(),
                     GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Uint32) * eboInd.size(),
                     eboInd.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                              (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(shaderProgram);
        // glPointSize(5.f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "MVP"), 1,
                           GL_FALSE, &MVP[0][0]);
        glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1,
                     &lineColor[0]);
        glUniform1i(glGetUniformLocation(shaderProgram, "w"), SCREEN_WIDTH);
        glUniform1i(glGetUniformLocation(shaderProgram, "h"), SCREEN_HEIGHT);

        glBindVertexArray(VAO);
        // std::cout<<v.size()<<std::endl;
        glDrawElements(GL_TRIANGLES, (v.size() / 6 - 1) * 6, GL_UNSIGNED_INT,
                       0);
    }
};