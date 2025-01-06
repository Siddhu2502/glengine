#version 460 core
out vec4 FragColor;

in vec4 vertexColor;
uniform vec4 myColor;

void main(){
    FragColor = myColor;
}