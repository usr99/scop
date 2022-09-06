# version 330 core
# define MAX_MATERIALS 128

struct Material
{
	vec4 ambientColor;
	vec4 diffuseColor;
	vec4 specularColor;
	
	float shininess;
	float refraction;
	float opacity;
	uint illum;	
};

layout (std140) uniform uMaterials
{
	Material materials[MAX_MATERIALS];
};

in vec3 currentPosition;
in vec3 normal;
in vec4 color;
in vec2 texCoord;
flat in uint materialId;

out vec4 fragColor;

uniform vec3 uLightPosition;
uniform vec3 uCameraPosition;
uniform sampler2D uTexture;
uniform float uTextureAlpha;

void main()
{
	Material mtl = materials[materialId];

	// model color
	vec4 texColor = texture(uTexture, texCoord);
	vec4 objectColor = (color * (1 - uTextureAlpha) + texColor * uTextureAlpha);
	
	// ligth direction
	vec3 normalVector = normalize(normal);
	vec3 lightDirection = normalize(uLightPosition - currentPosition);

	// ambient and diffuse lightings
	float ambient = 0.5;
	float diffuse = max(dot(normalVector, lightDirection), 0.0);

	// specular lighting
	float specularLight = 0.5;
	vec3 viewDirection = normalize(uCameraPosition - currentPosition);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0), mtl.shininess / 10);
	float specular = specAmount * specularLight;

	// final lighting color
	vec4 lightColor = mtl.diffuseColor * diffuse;
	if (mtl.illum > 0u)
		lightColor += mtl.ambientColor * ambient;
	if (mtl.illum > 1u)
		lightColor += mtl.specularColor * specular;

	// output color
	fragColor = objectColor * lightColor;
	fragColor.a = mtl.opacity;
}
