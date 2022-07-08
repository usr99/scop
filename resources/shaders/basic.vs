# version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

out vec4 Color;

void main()
{
	Color = vec4(aColor, 1.0);
	gl_Position = vec4(aPosition, 0.0, 1.0);
}
