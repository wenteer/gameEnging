#pragma once

#include <iostream>
#include <ctime>
#include <cstdio>
#include <random>

#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace std;

random_device rd;
mt19937 gen(rd());

class RandGenerator {
public:
    //todo Gererate random vec3
    static glm::vec3 genVec3() {
        std::uniform_real_distribution<float> distrib(-1, 1);

        float x = distrib(gen);
        float y = distrib(gen);
        float z = distrib(gen);

        return glm::vec3(x, y, z);
    }

    //todo Gererate random vec3
    static float genFloat() {
        std::uniform_real_distribution<float> distrib(-1, 1);
        return distrib(gen);
    }
};
