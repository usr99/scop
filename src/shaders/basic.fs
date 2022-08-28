# version 330 core

in vec3 currentPosition;
in vec3 normal;
in vec4 color;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 uLightPosition;
uniform sampler2D uTexture;
uniform bool uTextureEnabled;

void main()
{
	vec3 normalVector = normalize(normal);
	vec3 lightDirection = normalize(uLightPosition - currentPosition);

	float ambient = 1.0;
	float diffuse = max(dot(normalVector, lightDirection), 0.0);

	if (uTextureEnabled)
	{
		vec4 texColor = texture(uTexture, texCoord);
		fragColor = texColor * (diffuse + ambient);
	}
	else
	{
		fragColor = color * (diffuse + ambient);
	}
}
