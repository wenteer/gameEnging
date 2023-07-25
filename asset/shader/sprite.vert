#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// 将位移后的坐标、法线矩阵、纹理坐标传入fragment-shader
out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

// 各类矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal; // 法线矩阵mat3
    texCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
