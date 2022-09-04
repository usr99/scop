# version 330 core

in vec3 currentPosition;
in vec3 normal;
in vec4 color;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;
uniform sampler2D uTexture;
uniform float uTextureAlpha;

layout (std140) uniform uMaterials
{
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	
	float shininess;
	float refraction;
	float opacity;
	uint illum;	
};

void main()
{
	vec3 normalVector = normalize(normal);
	vec3 lightDirection = normalize(uLightPosition - currentPosition);

	float ambient = 1.0;
	float diffuse = max(dot(normalVector, lightDirection), 0.0);

	float specularLight = 0.5;
	vec3 viewDirection = normalize(uCameraPosition - currentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0), shininess / 10);
	float specular = specAmount * specularLight;

	vec4 texColor = texture(uTexture, texCoord);
	vec4 objectColor = (color * (1 - uTextureAlpha) + texColor * uTextureAlpha);
	vec4 lightColor = diffuseColor * diffuse;
	if (illum > 0u)
		lightColor += ambientColor * ambient;
	if (illum > 1u)
		lightColor += specularColor * specular;

	fragColor = objectColor * lightColor;
	fragColor.a = opacity;
}
