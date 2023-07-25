#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <helium/GameUtils.h>
#include <helium/SrcManager.h>

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

GameUtils game(SCREEN_WIDTH, SCREEN_HEIGHT);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

int main() {
    // Init glfw window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window
    GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Life is a funkin movie", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Callback function
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    // glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    // Initialize game
    game.Init();

    // DeltaTime variables
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Start Game within Menu State
    // breakout.state = GAME_ACTIVE;

    //*
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // Manage user input
        game.ProcessInput(deltaTime);

        // Update Game state
        game.Update(deltaTime);

        // Render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        game.Render();

        glfwSwapBuffers(window);
    }
    SrcManager::Clear();

    glfwTerminate();
    return 0;
}


void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if ((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if ((key >= 0) && (key < 1024)) {
        if (action == GLFW_PRESS) {
            game.keys[key] = GL_TRUE;
        }else if (action == GLFW_RELEASE) {
            game.keys[key] = GL_FALSE;
        }
    }
}
