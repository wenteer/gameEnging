#pragma once
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_img/stb_image.h>

#include <helium/TexUtils2D.h>
#include <helium/ShaderUtils.h>

class SrcManager {
public:
    // Resource storage
    static std::map<std::string, ShaderUtils> Shaders;
    static std::map<std::string, TexUtils2D> Textures;

    static ShaderUtils LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name) {
        Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
        return Shaders[name];
    }

    static ShaderUtils& GetShader(std::string name) {
        return Shaders[name];
    }

    static TexUtils2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name) {
        Textures[name] = loadTextureFromFile(file, alpha);
        return Textures[name];
    }

    static TexUtils2D& GetTexture(std::string name) {
        return Textures[name];
    }

    static void Clear() {
        // (Properly) delete all shaders
        for (auto iter : Shaders) {
            glDeleteProgram(iter.second.id);
        }
        // (Properly) delete all textures
        for (auto iter : Textures) {
            glDeleteTextures(1, &iter.second.id);
        }
    }

private:
    // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
    SrcManager() {}
    // Loads and generates a shader from file
    static ShaderUtils loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile = nullptr) {
        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::string geometryCode;

        try {
            // Open files
            std::ifstream vertexShaderFile(vShaderFile);
            std::ifstream fragmentShaderFile(fShaderFile);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vertexShaderFile.rdbuf();
            fShaderStream << fragmentShaderFile.rdbuf();
            // close file handlers
            vertexShaderFile.close();
            fragmentShaderFile.close();
            // Convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
            // If geometry shader path is present, also load a geometry shader
            if (gShaderFile != nullptr) {
                std::ifstream geometryShaderFile(gShaderFile);
                std::stringstream gShaderStream;
                gShaderStream << geometryShaderFile.rdbuf();
                geometryShaderFile.close();
                geometryCode = gShaderStream.str();
            }
        }
        catch (std::exception e) {
            std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
        }
        const GLchar *vShaderCode = vertexCode.c_str();
        const GLchar *fShaderCode = fragmentCode.c_str();
        const GLchar *gShaderCode = geometryCode.c_str();
        // 2. Now create shader object from source code
        ShaderUtils shader;

        shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
        return shader;
    }

    // Loads a single texture from file
    static TexUtils2D loadTextureFromFile(const GLchar *file, GLboolean alpha) {
        TexUtils2D texture;

        if (alpha) { 
            texture.internalFormat = GL_RGBA;
            texture.imageFormat = GL_RGBA;
        }
        int width, height, nrChannels;

        // stbi_set_flip_vertically_on_load(true);
        unsigned char *img = stbi_load(file, &width, &height, &nrChannels, 0);

        // Now generate texture
        texture.generate(width, height, img);
        // And finally free image data
        stbi_image_free(img);
        return texture;
    }
};

std::map<std::string, ShaderUtils> SrcManager::Shaders;
std::map<std::string, TexUtils2D> SrcManager::Textures;
