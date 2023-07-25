#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <tuple>

#include <helium/SrcManager.h>
#include <helium/Sprite.h>
#include <helium/GameObject.h>
#include <helium/Camera.h>
#include <helium/Particles/ParticleGenerator.h>

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

//* Globle Game object defined
Sprite *renderer;
ParticleGenerator *pg;

GameObject *box;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

class GameUtils {
public:
    // Attribute
    GameState state;
    GLboolean keys[1024];
    GLuint width, height;

    // Constructor & Deconstructor
    GameUtils(GLuint width, GLuint height) :
        state(GAME_ACTIVE),
        keys(),
        width(width),
        height(height) {}

    ~GameUtils() {
    }

    // initial level
    void Init() {
        // ***************************** 加载所有游戏资源
        // Load shaders
        SrcManager::LoadShader(
            "../asset/shader/sprite.vert",
            "../asset/shader/sprite.frag",
            nullptr,
            "sprite");
        SrcManager::LoadShader(
            "../asset/shader/particle.vert",
            "../asset/shader/particle.frag",
            nullptr,
            "particle");

        // Set matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)width / (float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.getViewMatrix();

        // Set uniform var
        SrcManager::GetShader("sprite").use();
        SrcManager::GetShader("sprite").SetMatrix4("view", view);
        SrcManager::GetShader("sprite").SetMatrix4("projection", projection);

        SrcManager::GetShader("particle").use();
        SrcManager::GetShader("particle").SetMatrix4("view", view);
        SrcManager::GetShader("particle").SetMatrix4("projection", projection);

        // Load texture
        SrcManager::LoadTexture("../asset/img/ball1.png", GL_TRUE, "face");
        SrcManager::LoadTexture("../asset/img/brick.png", GL_TRUE, "brick");
        SrcManager::LoadTexture("../asset/img/block_solid.png", GL_FALSE, "block_solid");
        SrcManager::LoadTexture("../asset/img/paddle.png", GL_TRUE, "paddle");
        SrcManager::LoadTexture("../asset/img/background/b3.png", GL_FALSE, "background");
        SrcManager::LoadTexture("../asset/img/particle1.png", GL_TRUE, "circle");

        // Load renderer
        renderer = new Sprite(SrcManager::GetShader("sprite"));

        // ***************************** 加载结束 *****************************
        Particle p(
            glm::vec3(0.0f),
            glm::vec3(0.02f),
            glm::vec3(1.0f),
            glm::vec3(0.0f, -0.2f, 0.0f),
            1.0f);

        pg = new ParticleGenerator(
            5000,
            "particle",
            "circle",
            p);

        box = new GameObject(
            glm::vec3(0.0f),
            glm::vec3(0.1f),
            SrcManager::GetTexture("block_solid"),
            glm::vec3(0.0f, 0.8f, 0.6f)
            );
    }

    void ProcessInput(GLfloat dt) {
        if (this->keys[GLFW_KEY_A]) {
            camera.ProcessKeyboard(LEFT, dt);
        }
        if (this->keys[GLFW_KEY_D]) {
            camera.ProcessKeyboard(RIGHT, dt);
        }
        if (this->keys[GLFW_KEY_W]) {
            camera.ProcessKeyboard(FORWARD, dt);
        }
        if (this->keys[GLFW_KEY_S]) {
            camera.ProcessKeyboard(BACKWARD, dt);
        }
        if (this->keys[GLFW_KEY_SPACE]) {
        }
    }

    void Update(GLfloat deltaTime) {
        pg->update(deltaTime, 12);
        box->circle(deltaTime);
    }

    void Render() {
        pg->draw();
        box->draw(*renderer);
    }
};
