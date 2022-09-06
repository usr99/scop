# version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aTexture;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aColor;
layout(location = 4) in float aMaterialIndex;

out vec3 currentPosition;
out vec3 normal;
out vec4 color;
out vec2 texCoord;
flat out uint materialId;
out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 uCamera;
uniform mat4 uModel;

void main()
{
	currentPosition = vec3(uModel * vec4(aPosition, 1.0));
	normal = (uModel * vec4(aNormal, 1.0)).xyz;
	texCoord = aTexture.xy;
	color = vec4(aColor, 1.0);
	materialId = uint(aMaterialIndex);

	worldPosition = vec3(uModel * vec4(aPosition, 1.0));
	worldNormal = mat3(uModel) * aNormal;

	gl_Position = uCamera * vec4(currentPosition, 1.0);
}
