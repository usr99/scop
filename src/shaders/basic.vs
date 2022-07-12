# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aTexture;
layout(location = 2) in vec3 aNormal;

out vec4 fragColor;

uniform mat4 uMVP;

void main()
{
	fragColor = vec4(1.0, 1.0, 1.0, 1.0);
	gl_Position = uMVP * vec4(aPosition, 1.0);
}
