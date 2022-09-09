# version 330 core
# define MAX_MATERIALS 128
# define MAX_TEXTURES 16

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
uniform sampler2D uTexture[MAX_TEXTURES];
uniform float uTextureAlpha;

void main()
{
	Material mtl = materials[materialId];

	vec4 texColor;
	switch (materialId)
	{
		case 0u:	
			texColor = texture(uTexture[0], texCoord); break;
		case 1u:
			texColor = texture(uTexture[1], texCoord); break;
		case 2u:
			texColor = texture(uTexture[2], texCoord); break;
		case 3u:
			texColor = texture(uTexture[3], texCoord); break;
		case 4u:
			texColor = texture(uTexture[4], texCoord); break;
		case 5u:
			texColor = texture(uTexture[5], texCoord); break;
		case 6u:
			texColor = texture(uTexture[6], texCoord); break;
		case 7u:
			texColor = texture(uTexture[7], texCoord); break;
		case 8u:
			texColor = texture(uTexture[8], texCoord); break;
		case 9u:
			texColor = texture(uTexture[9], texCoord); break;
		case 10u:
			texColor = texture(uTexture[10], texCoord); break;
		case 11u:
			texColor = texture(uTexture[11], texCoord); break;
		case 12u:
			texColor = texture(uTexture[12], texCoord); break;
		case 13u:
			texColor = texture(uTexture[13], texCoord); break;
		case 14u:
			texColor = texture(uTexture[14], texCoord); break;
		case 15u:
			texColor = texture(uTexture[15], texCoord); break;
	}

	// model color
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
