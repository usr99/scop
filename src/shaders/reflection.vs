# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aTexture;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aColor;
layout(location = 4) in float aMaterialIndex;

uniform mat4 uCamera;
uniform mat4 uModel;

flat out uint materialId;
out vec3 worldPosition;
out vec3 worldNormal;

void main()
{
	materialId = uint(aMaterialIndex);
	worldPosition = vec3(uModel * vec4(aPosition, 1.0));
	worldNormal = mat3(uModel) * aNormal;

	gl_Position = uCamera * uModel * vec4(aPosition, 1.0);
}
