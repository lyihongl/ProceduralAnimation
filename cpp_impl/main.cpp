#include <cstdio>
#include <cstdlib>
#include <shaders.hpp>
#include <windows.h>

#define GLM_FORCE_RADIANS 1
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <random>

#include "global.hpp"
#include "camera.hpp"
// #include "line.hpp"
#include "line2d.hpp"
#include "point.hpp"
#include "snake.hpp"

const int SCREEN_FULLSCREEN = 0;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const float VP_RATIO = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
SDL_Window *window = NULL;
SDL_GLContext maincontext;

uint32_t frame_count = 0;
// Camera camera;

static void sdl_die(const char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void init_screen(const char *caption) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) sdl_die("Couldn't initialize SDL");
    atexit(SDL_Quit);
    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create the window
    if (SCREEN_FULLSCREEN) {
        window = SDL_CreateWindow(
            caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        window = SDL_CreateWindow(caption, SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    }
    if (window == NULL) sdl_die("Couldn't set video mode");

    maincontext = SDL_GL_CreateContext(window);
    if (maincontext == NULL) sdl_die("Failed to create OpenGL context");

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // Use v-sync
    // SDL_GL_SetSwapInterval(1);

    // Disable depth test and face culling.
    // glShadeModel(GL_FLAT)
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_SetSwapInterval(0);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.5f, 1.0f, 0.0f);
}

int main(int argc, char **argv) {

    init_screen("OpenGL 4.3");
    SDL_Event event;
    bool quit = false;
    bool be_nice_and_dont_burn_the_cpu = true;
    // Uint32 time_step_ms = 1000 / 60;
    // Uint32 next_game_step = SDL_GetTicks(); // initial value
    // std::string x;
    Uint32 minimum_fps_delta_time =
        (1000 /
         60); // minimum 6 fps, if the computer is slower than this: slow down.
    Uint32 last_game_step = SDL_GetTicks(); // initial value
    Shader s("./shaders/lineShader.vert", "./shaders/lineShader.frag");
    Shader p("./shaders/pointShader.vert", "./shaders/pointShader.frag");
    Shader t("./shaders/triagShader.vert", "./shaders/triagShader.frag");

    // camera.position = glm::vec3(3, 3, 3);

    // 3d lines example

    // Line line1(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0));
    // line1.shaderProgram = s.ID;
    // line1.setColor(glm::vec3(1, 0, 0));
    // Line line2(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    // line2.shaderProgram = s.ID;
    // line2.setColor(glm::vec3(0, 1, 0));
    // Line line3(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    // line3.shaderProgram = s.ID;
    // line3.setColor(glm::vec3(0, 0, 1));
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f,
        100.0f);

    float angle = 0.0f;
    float rotationSpeed = 0.1f;

    // 2d line example
    glm::vec2 start = glm::vec2(10, 10);
    glm::vec2 end = glm::vec2(10, 40);

    // float x1 = start.x;
    // float y1 = start.y;
    // float x2 = end.x;
    // float y2 = end.y;
    // float w = SCREEN_WIDTH;
    // float h = SCREEN_HEIGHT;

    // // convert 3d world space position 2d screen space position
    // x1 = 2 * x1 / w - 1;
    // y1 = 2 * y1 / h - 1;

    // x2 = 2 * x2 / w - 1;
    // y2 = 2 * y2 / h - 1;

    // start.x = x1;
    // start.y = y1;
    // end.x = x2;
    // end.y = y2;

    Line2DRender line{};
    Point2DRender point{};
    SnakeRender2D snakePoints{line};
    // line.addLine(start, end);
    // line.addLine(end, glm::vec2(100.f, 100.f));
    // line.addLine(glm::vec2(100.f, 100.f), glm::vec2(200.f, 100));
    // line.shaderProgram = s.ID;
    // Point2D point(glm::vec3(100, 100, 0), glm::vec3(200, 200, 0));
    line.shaderProgram = s.ID;
    point.shaderProgram = p.ID;

    point.addPoint({100.f, 100.f});

    Snake snake(glm::vec2(40.f, 40.f));
    std::vector<unsigned int> snakeP;
    for (int i = 0; i < 15; i++) {
        snakeP.push_back(snake.addPoint());
    }

    for (int i = 1; i < 15; i++) {
        snake.addConnection(snakeP[i - 1], snakeP[i], 30.f);
    }
    SnakeMesh2D snakeMesh{};
    snakeMesh.shaderProgram = t.ID;
    // snake.initPlacement();
    // for (auto &p : snake.points) {
    //     std::cout << "points: " << glm::to_string(p) << std::endl;
    // }

    // point.shaderProgram = p.ID;

    // Line line4(start, end);
    // line4.setColor()
    // line4.shaderProgram = s.ID;

    int mX = 0, mY = 0;
    while (!quit) {
        Uint32 now = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                // if (rng_gen) delete rng_gen;
                quit = true;
                break;
            }
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_q) {
                    quit = true;
                    break;
                }
                // for(auto &p : snake.points){
                //     std::cout<<"moveP: "<<glm::to_string(p)<<std::endl;
                // }
                // for(auto &drawn : drawnLines){
                //     std::cout << drawn << std::endl;
                // }
                // std::cout << "--" << std::endl;
            }
                // case SDL_MOUSEMOTION: {
                // break;
                // }
                // case SDL_WINDOWEVENT_SIZE_CHANGED: {
                //     w.updateDim(event.window.data1, event.window.data2);
                //     break;
                // }
            }
        }

        if (last_game_step < now) {
            SDL_GetMouseState(&mX, &mY);
            SDL_GL_SwapWindow(window);

            Uint32 delta_time = now - last_game_step;

            if (delta_time > minimum_fps_delta_time) {
                delta_time = minimum_fps_delta_time; // slow down if the
                                                     // computer is too slow
            }
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            // snake.moveStart({mX, SCREEN_HEIGHT - mY});
            // std::cout << (1000/delta_time)<<std::endl;

            // AdvanceGameLogicBy(delta_time);
            // angle += delta_time * rotationSpeed;

            // s.Use();
            // update camera position (rotating)
            // camera.position = glm::vec3(3 * cos(glm::radians(angle)), 3,
            //                             3 * sin(glm::radians(angle)));
            // glm::mat4 view = glm::lookAt(camera.position, glm::vec3(0, 0, 0),
            //                              glm::vec3(0, 1, 0));

            // line1.setMVP(projection * view);
            // line2.setMVP(projection * view);
            // line3.setMVP(projection * view);

            // line1.draw();
            // line2.draw();
            // line3.draw();

            // line4.draw();
            snake.moveStart({mX, SCREEN_HEIGHT - mY});
            snakePoints.computePoints(snake);
            // line.debugPrint();
            std::unordered_set<int> drawnLines;

            line.clear();
            for (auto &[c, v] : snake.connections) {
                for (auto &otherT : snake.connections[c]) {
                    int other = otherT.first;
                    if (drawnLines.find(c * other) != drawnLines.end())
                        continue;
                    drawnLines.insert(c * other);
                    // drawnLines.insert();
                    line.addLine(snake.points[c], snake.points[other]);
                    // std::cout << "adding: "<<c<<" "<<other<<std::endl;
                    // std::cout << "adding: " <<
                    // glm::to_string(snake.points[c])
                    //           << ", " <<
                    //           glm::to_string(snake.points[other])
                    //           << std::endl;
                }
            }
            line.prepareDraw();

            line.draw();
            snakeMesh.draw(snakePoints.vertices);
            point.drawExternal(snakePoints.vertices);
            // point.drawExternal({{100.f, 100.f}});
            // point.prepareDraw();
            // point.draw();
            // point.draw();

            last_game_step = now;

            // RenderGame();
        } else {
            // we're too fast, wait a bit.
            if (be_nice_and_dont_burn_the_cpu) {
                SDL_Delay(1);
            }
        }
    }
    return 0;
}