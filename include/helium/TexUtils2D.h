#pragma once

#include <glad/glad.h>

class TexUtils2D {
public:
    GLuint id;
    GLuint width, height;
    GLuint internalFormat;
    GLuint imageFormat;

    GLuint wrapS;
    GLuint wrapT;
    GLuint filterMin;
    GLuint filterMax;

    TexUtils2D() :
        width(0),
        height(0),
        internalFormat(GL_RGB),
        imageFormat(GL_RGB),
        wrapS(GL_REPEAT),
        wrapT(GL_REPEAT),
        filterMin(GL_LINEAR),
        filterMax(GL_LINEAR) {
        glGenTextures(1, &this->id);
    }

    //* generate texture
    void generate(GLuint width, GLuint height, unsigned char *data) {
        this->width = width;
        this->height = height;

        // Create Texture
        glBindTexture(GL_TEXTURE_2D, this->id);
        // Set Texture wrap and filter modes
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMax);

        glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->imageFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Unbind texture
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    //* set bind to const to prevent texture-members from being modified
    void bind() const {
        glBindTexture(GL_TEXTURE_2D, this->id);
    }
};