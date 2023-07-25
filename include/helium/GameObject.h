#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <helium/TexUtils2D.h>
#include <helium/Sprite.h>

class GameObject {
public:
    // Object state
    glm::vec3 pos, size, speed;
    glm::vec3 color;
    GLfloat rotate;

    // Render state
    TexUtils2D texture;

    // Constructor
    GameObject() :
        pos(glm::vec3(0)),
        size(glm::vec3(100.0f)),
        speed(0.0f),
        color(glm::vec3(1.0f)),
        texture() {
    }

    GameObject(glm::vec3 pos, glm::vec3 size, TexUtils2D texture, glm::vec3 color) :
        pos(pos),
        size(size),
        rotate(0.0f),
        texture(texture),
        color(color) {
    }

    // Draw sprite
    virtual void draw(Sprite& renderer) {
        renderer.drawSprite(this->pos, this->rotate, this->size, this->texture, this->color);
    }

    void circle(float dt) {
        this->rotate += dt * 50;
    }
};
