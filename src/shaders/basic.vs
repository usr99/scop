# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aTexture;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aColor;

out vec3 currentPosition;
out vec3 normal;
out vec4 color;

uniform mat4 uCamera;
uniform mat4 uModel;

void main()
{
	currentPosition = vec3(uModel * vec4(aPosition, 1.0));
	normal = aNormal;
	color = vec4(aColor, 1.0);

	gl_Position = uCamera * vec4(currentPosition, 1.0);
}
