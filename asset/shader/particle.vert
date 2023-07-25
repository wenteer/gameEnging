#version 330 core
layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 TexCoor;

out vec2 fTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    fTexCoords = TexCoor;
    gl_Position = projection * view * model  * vec4(Pos, 1.0);
}