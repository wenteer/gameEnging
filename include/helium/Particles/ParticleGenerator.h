#pragma once
#include <vector>
#include <random>
#include <string>
#include <ctime>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <helium/ShaderUtils.h>
#include <helium/TexUtils2D.h>
#include <helium/SrcManager.h>
#include <helium/RandGenerator.h>

struct Particle {
    glm::vec3 pos;
    glm::vec3 size;
    glm::vec3 rotate;
    glm::vec3 color;
    glm::vec3 speed;
    GLfloat   life;

    Particle(const Particle& p) :
        pos(p.pos),
        size(p.size),
        rotate(p.rotate),
        color(p.color),
        speed(p.speed),
        life(p.life) {}

    Particle(glm::vec3 pos, glm::vec3 size, glm::vec3 color, glm::vec3 speed, GLfloat life) :
        pos(pos),
        size(size),
        rotate(rotate),
        color(color),
        speed(speed),
        life(life) {}
};

class ParticleGenerator {
private:
    // 向量、数量、shader、vao
    Particle particle;
    std::vector<Particle> particles;
    GLuint amount;

    std::string shader;
    std::string texture;
    GLuint vao;

    void init() {
        GLuint vbo;
        GLfloat vertices[] = {
            // positions         // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,   0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,   0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f,  -0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,   0.5f,  0.5f, 1.0f, 1.0f,
            0.5f,   0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

            0.5f,   0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,   0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f,   0.5f,  0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  -0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,  -0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,   0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,   0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,   0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f
        };

        glGenBuffers(1, &vbo);
        glGenVertexArrays(1, &this->vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindVertexArray(this->vao);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

        glBindVertexArray(0);

        // 初始化所有粒子对象，每个粒子对象所有数据重置
        for (GLuint i = 0; i < this->amount; i++) {
            Particle p = Particle(this->particle);
            p.life = 0.0f;
            this->particles.push_back(p);
        }
    }

    // 找到最近一个生命耗尽的粒子
    GLuint lastUsed = 0;
    GLuint firstUnusedParticle() {
        for (GLuint i = lastUsed; i < this->amount; i++) {
            if (this->particles[i].life <= 0.0f) {
                lastUsed = i;
                return i;
            }
        }
        for (GLuint i = 0; i < this->amount; i++) {
            if (this->particles[i].life <= 0.0f) {
                lastUsed = i;
                return i;
            }
        }

        lastUsed = 0;
        return 0;
    }

    //! 重新产生粒子
    void spawnParticle(int i) {
        glm::vec3 bias = RandGenerator::genVec3();
        particles[i].pos = this->particle.pos + bias;
        particles[i].size = this->particle.size;
        particles[i].rotate = this->particle.rotate;
        particles[i].color = this->particle.color;
        particles[i].speed = this->particle.speed;
        particles[i].life = this->particle.life;
    }

    // 移动粒子
    void move(GLfloat deltaTime, Particle& p) {
        p.pos += p.speed * deltaTime;
    }

public:
    ParticleGenerator(GLuint amount, std::string shader, std::string texture, Particle& particle) :
        amount(amount),
        shader(shader),
        texture(texture),
        particle(particle) {
        this->init();
    }

    void update(GLfloat deltaTime, GLuint count) {
        // 产生新粒子
        for (int i = 0; i < count; i++) {
            int unusedParticle = this->firstUnusedParticle();
            printf("new particle generated no.%d\n",unusedParticle);
            spawnParticle(unusedParticle);
        }
        // 更新所有粒子状态
        for (int i = 0; i < this->amount; i++) {
            this->move(deltaTime, particles[i]);
            particles[i].life -= deltaTime;
        }
    }

    void draw() {
        for (auto p:this->particles) {
            if (p.life > 0.0) {
                SrcManager::GetShader(shader).use();
                glm::mat4 model = glm::mat4(1.0f);
                // 移动
                model = glm::translate(model, p.pos);
                // 旋转
                // model = glm::rotate(model, p.rotate, glm::vec3(0.0f, 0.0f, 1.0f));
                // 缩放
                model = glm::scale(model, p.size);

                SrcManager::GetShader(shader).SetMatrix4("model", model);
                SrcManager::GetShader(shader).SetVector3f("color", p.color);

                SrcManager::GetTexture(texture).bind();
                glBindVertexArray(this->vao);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }
    }
};
