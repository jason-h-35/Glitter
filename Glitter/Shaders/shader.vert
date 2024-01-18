#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform vec3 xOffset;
uniform vec3 colorVal;

out vec3 ourColor; //output color to frag shader

void main()
{
    gl_Position = vec4(aPos+xOffset, 1.0);
    ourColor = aColor+colorVal;
}

