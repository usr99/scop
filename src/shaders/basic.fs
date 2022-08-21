# version 330 core

in vec3 currentPosition;
in vec3 normal;
in vec4 color;

out vec4 fragColor;

uniform vec3 uLightColor;
uniform vec3 uLightPosition;

void main()
{
	vec4 lightColor = vec4(uLightColor, 1.0);
	vec3 normalVector = normalize(normal);
	vec3 lightDirection = normalize(uLightPosition - currentPosition);

	float ambient = 0.4;
	float diffuse = max(dot(normalVector, lightDirection), 0.0);

	fragColor = color * lightColor * (diffuse + ambient);
}
