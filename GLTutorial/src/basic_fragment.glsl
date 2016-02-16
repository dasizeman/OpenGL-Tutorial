#version 150

uniform vec3 triangleColor;
out vec4 outColor;

// Comment
void main()
{
    outColor = vec4(triangleColor, 1.0);
}
