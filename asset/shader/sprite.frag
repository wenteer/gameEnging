#version 330 core

in vec2 texCoords;

out vec4 FragColor;

uniform vec3 spriteColor;
uniform sampler2D diffuse;

void main(){
    FragColor = texture(diffuse, texCoords) * vec4(spriteColor, 1.0);
}
