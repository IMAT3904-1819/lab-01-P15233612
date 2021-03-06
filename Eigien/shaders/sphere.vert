#version 330
layout (location = 0) in vec3 aPos;
out vec3 vPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
 
void main(void)
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vPosition = aPos;
}