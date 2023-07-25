#pragma once
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class ShaderUtils {
private:
    void compileDebug(GLuint object, std::string type){
         GLint success;
        GLchar infoLog[1024];

        if (type != "PROGRAM") {
            glGetShaderiv(object, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- "
                        << std::endl;
            }
        } else {
            glGetProgramiv(object, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(object, 1024, NULL, infoLog);
                std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                        << infoLog << "\n -- --------------------------------------------------- -- "
                        << std::endl;
            }
        }
    }

public:
    //* Program id
    GLuint id;
    
    //* Constructor
    ShaderUtils() {}
   
    //* Sets the current shader as active
    ShaderUtils& use(){
        glUseProgram(this->id);
        return *this;
    }

    //* Compiles the shader from given source code
    void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr){
        GLuint sVertex, sFragment, gShader;

        //* Vertex shader
        sVertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(sVertex, 1, &vertexSource, NULL);
        glCompileShader(sVertex);
        compileDebug(sVertex, "VERTEX");
        //* Fragment shader
        sFragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(sFragment, 1, &fragmentSource, NULL);
        glCompileShader(sFragment);
        compileDebug(sFragment, "FRAGMENT");
        //* If geometry shader source code is given, also compile geometry shader
        if (geometrySource != nullptr) {
            gShader = glCreateShader(GL_GEOMETRY_SHADER);
            glShaderSource(gShader, 1, &geometrySource, NULL);
            glCompileShader(gShader);
            compileDebug(gShader, "GEOMETRY");
        }
        //* Attache to shader program
        this->id = glCreateProgram();
        glAttachShader(this->id, sVertex);
        glAttachShader(this->id, sFragment);
        if (geometrySource != nullptr) {
            glAttachShader(this->id, gShader);
        }
        glLinkProgram(this->id);
        compileDebug(this->id, "PROGRAM");

        //* Delete the shaders
        glDeleteShader(sVertex);
        glDeleteShader(sFragment);
        if (geometrySource != nullptr) {
            glDeleteShader(gShader);
        }
    }

    //* Set uniform var
    void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader) {
        if (useShader) {
            this->use();
        }
        glUniform1f(glGetUniformLocation(this->id, name), value);
    }


    void SetInteger(const GLchar *name, GLint value, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform1i(glGetUniformLocation(this->id, name), value);
    }


    void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform2f(glGetUniformLocation(this->id, name), x, y);
    }


    void SetVector2f(const GLchar *name, const glm::vec2& value, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
    }


    void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
    }


    void SetVector3f(const GLchar *name, const glm::vec3& value, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
    }


    void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
    }


    void SetVector4f(const GLchar *name, const glm::vec4& value, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
    }


    void SetMatrix4(const GLchar *name, const glm::mat4& matrix, GLboolean useShader = false) {
        if (useShader) {
            this->use();
        }
        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(matrix));
    }
};
