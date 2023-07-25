#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <helium/TexUtils2D.h>
#include <helium/ShaderUtils.h>
 
class Sprite {
public:
    //* Constructor (inits shaders/shapes)
    Sprite(ShaderUtils& shader) {
        this->shader = shader;
        this->initRenderData();
    }

    //* Destructor
    ~Sprite() {
        glDeleteVertexArrays(1, &this->VAO);
    }

    //* Renders a defined quad textured with given sprite
    void drawSprite(glm::vec3 position, GLfloat rotate, glm::vec3 size, TexUtils2D& texture, glm::vec3 color) {
        // Prepare transformations
        this->shader.use();

        glm::mat4 model = glm::mat4(1.0f);

        model = glm::translate(model, position);                             // First translate (transformations are: scale happens first, then rotation and then finall translation happens; reversed order)

        // model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));   // Move origin of rotation to center of quad
        model = glm::rotate(model, glm::radians(rotate), glm::vec3(1.0f, 1.0f, 1.0f));  // Then rotate
        // model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f)); // Move origin back

        model = glm::scale(model, size);

        this->shader.SetMatrix4("model", model);

        this->shader.SetVector3f("spriteColor", color);

        glActiveTexture(GL_TEXTURE0);
        texture.bind();

        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

private:
    // Render state
    ShaderUtils shader;
    GLuint VAO;
    // Initializes and configures the quad's buffer and vertex attributes
    void initRenderData() {
        // VBO对象和 3d cube 顶点数据
        GLuint VBO;
        GLfloat vertices[] = {
            // positions          // normals          // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
            0.5f,  -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
            0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            0.5f,   0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
            0.5f,  -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
            0.5f,   0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            0.5f,   0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

            0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
            0.5f,   0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
            0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  -0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
            0.5f,   0.5f,  0.5f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
            0.5f,  -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
            0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            0.5f,  -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
            0.5f,   0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
            0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            0.5f,   0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
        };

        // 创建 顶点缓冲 和 顶点数组 对象
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &VBO);

        // 导入顶点数据
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 顶点属性规范
        glBindVertexArray(this->VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //解绑对象
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
