# version 330 core

in vec3 currentPosition;
in vec3 normal;
in vec4 color;

out vec4 fragColor;

void main()
{
	vec3 normalVector = normalize(normal);
	vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 lightPosition = vec3(0.0, 0.0, -1.0);
	vec3 lightDirection = normalize(lightPosition - currentPosition);

	float ambient = 0.2;
	float diffuse = max(dot(normalVector, lightDirection), 0.0);

	fragColor = color * lightColor * (diffuse + ambient);
}
