#version 460 core

out vec4 FragColor;

in vec3 myColor;
in vec2 TexCoord;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;

void main()
{
    // FragColor = texture(myTexture, TexCoord) * vec4(myColor, 1.0) - vec4(0.3412, 0.0, 0.0, 0.0);
    FragColor = mix(texture(myTexture1, TexCoord)*vec4(myColor, 0.2), 
                    texture(myTexture2, TexCoord)*vec4(myColor, 0.6), 0.4);
}