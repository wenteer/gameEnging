#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//* 相机的移动枚举类型
enum cameraMove {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//* 默认的参数设置
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 4.5f;
const float SENSITIVITY = 0.05f;
const float ZOOM = 45.0f;

class Camera {
public:
    //*  相机自身参数
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    //*  相机旋转参数
    float yaw;
    float pitch;
    //*  速度、鼠标灵敏度、缩放
    float speed;
    float sensitivity;
    float zoom;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = YAW,
           float pitch = PITCH) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)),
        speed(SPEED),
        sensitivity(SENSITIVITY),
        zoom(ZOOM) {
        this->pos = pos;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) :
        front(glm::vec3(0.0f, 0.0f, -1.0f)), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
        this->pos = glm::vec3(posX, posY, posZ);
        this->worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        pitch = pitch;
        updateCameraVectors();
    }

    //* 根据当前的相机参数，生成一个view矩阵
    glm::mat4 getViewMatrix() {
        return glm::lookAt(this->pos, this->pos + this->front, this->up);
    }

    //* 根据枚举类型计算新的位置
    void ProcessKeyboard(cameraMove direction, float deltaTime) {
        float velocity = this->speed * deltaTime;

        if (direction == FORWARD) {
            this->pos += this->front * velocity;
        }
        if (direction == BACKWARD) {
            this->pos -= this->front * velocity;
        }
        if (direction == LEFT) {
            this->pos -= this->right * velocity;
        }
        if (direction == RIGHT) {
            this->pos += this->right * velocity;
        }
        if (direction == UP) {
            this->pos += this->up * velocity;
        }
        if (direction == DOWN) {
            this->pos -= this->up * velocity;
        }
    }

    //* 计算鼠标的移动数据
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
        xOffset *= this->sensitivity;
        yOffset *= this->sensitivity;

        this->yaw += xOffset;
        this->pitch += yOffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (this->pitch > 89.0f) {
                this->pitch = 89.0f;
            }
            if (this->pitch < -89.0f) {
                this->pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset) {
        this->zoom -= (float)yOffset;
        if (this->zoom < 1.0f) {
            this->zoom = 1.0f;
        }
        if (this->zoom > 45.0f) {
            this->zoom = 45.0f;
        }
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;

        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        this->right = glm::normalize(glm::cross(this->front, this->worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        this->up = glm::normalize(glm::cross(this->right, this->front));
    }
};
