#version 460

layout(location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 outputColor;
void main()
{
    gl_Position = vec4(position.xy, 0.0, 1.0);
    outputColor = color;
}