#version 330 core

out vec4 f_color;

uniform vec3 color;

void main()
{
    f_color = vec4(color, 1.0f);
}