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

uniform vec3 uCameraPosition;
uniform samplerCube uCubemap;
uniform bool uRefractionEnabled;

flat in uint materialId;
in vec3 worldPosition;
in vec3 worldNormal;

out vec4 fragColor;

void main()
{
	Material mtl = materials[materialId];

	vec3 incident = normalize(worldPosition - uCameraPosition);
	vec3 n = normalize(worldNormal);

	if (uRefractionEnabled)
	{
		vec3 refracted = refract(incident, n, 1 / mtl.refraction);
		fragColor = texture(uCubemap, refracted);
	}
	else
	{
		vec3 reflected = reflect(incident, n);
		fragColor = texture(uCubemap, reflected);
	}
}
