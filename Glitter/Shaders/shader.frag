#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // gets set in the C++ source code

void main()
{
    FragColor = ourColor;
}

