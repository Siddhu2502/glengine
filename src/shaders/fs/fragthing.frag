#version 460 core

out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D myTexture;

void main()
{
    FragColor = texture(myTexture, TexCoord) * vec4(myColor, 1.0) - vec4(0.3412, 0.0, 0.0, 0.0);
}