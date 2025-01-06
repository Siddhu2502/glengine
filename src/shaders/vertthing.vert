#version 460 core
layout(location = 0) in vec3 aPos;// position has attribute position 0
layout(location = 1) in vec3 aColor; // color has attribute position 1

out vec3 myColor;

void main()
{
    gl_Position = vec4(aPos,1.);// we give a vec3 to vec4’s constructor
    myColor = aColor;//output variable to dark-red
}