#version 330 core

in vec2 fTexCoords;

out vec4 fragColor;

uniform sampler2D diffuse;
uniform vec3 color;

void main()
{
    fragColor = texture(diffuse, fTexCoords) * vec4(color, 1.0);
}