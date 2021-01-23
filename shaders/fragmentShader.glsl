#version 460

out vec4 FragColor;
in vec3 outputColor;
void main()
{
    FragColor = vec4(outputColor, 1.0);
} 