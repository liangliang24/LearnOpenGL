#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};

uniform Material u_Material;

struct PointLight
{
	vec3 position;
	vec3 color;

	float ambient;
	float diffuse;
	float specular;
};

uniform PointLight u_PointLight;

struct DirectionLight
{
	vec3 direction;
	vec3 color;

	float ambient;
	float diffuse;
	float specular;
};

uniform DirectionLight u_DirectionLight;

struct SpotLight
{
	vec3 color;
	vec3 position;
	vec3 direction;
	float cutOff;
	float outCutOff;

	float ambient;
	float diffuse;
	float specular;
};

uniform SpotLight u_SpotLight;

uniform vec3 u_CameraPos;

vec3 CalculatePointLight(PointLight pointLight, Material material)
{
	float distance = length(pointLight.position - FragPos);
	float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);

	vec3 ambient = pointLight.ambient * pointLight.color * vec3(texture(material.diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(pointLight.position - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * vec3(texture(material.diffuse, TexCoords)) * pointLight.color * pointLight.diffuse;

	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, halfwayDir), 0.0), material.shininess) * vec3(texture(material.specular, TexCoords)) * pointLight.color * pointLight.specular;

	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateDirectionLight(DirectionLight directionLight, Material material)
{
	vec3 ambient = directionLight.ambient * directionLight.color * vec3(texture(material.diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(-directionLight.direction);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * vec3(texture(material.diffuse, TexCoords)) * directionLight.color * directionLight.diffuse;

	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, halfwayDir), 0.0), material.shininess) * vec3(texture(material.specular, TexCoords)) * directionLight.color * directionLight.specular;

	return ambient + diffuse + specular;
}

vec3 CalculateSpotLight(SpotLight spotLight, Material material)
{
	vec3 lightDir = normalize(spotLight.position - FragPos);

	float theta = dot(lightDir, normalize(-spotLight.direction));

	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0f / (1.0f + 0.09f * distance + 0.032f * distance * distance);

	float epsilon = (spotLight.cutOff - spotLight.outCutOff);
	float intensity = clamp((theta - spotLight.outCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient = spotLight.ambient * spotLight.color * vec3(texture(material.diffuse, TexCoords));

	vec3 norm = normalize(Normal);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * vec3(texture(material.diffuse, TexCoords)) * spotLight.color * spotLight.diffuse;

	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, halfwayDir), 0.0), material.shininess) * vec3(texture(material.specular, TexCoords)) * spotLight.color * spotLight.specular;
		
	return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main()
{
	vec3 result = CalculatePointLight(u_PointLight, u_Material);
	result += CalculateDirectionLight(u_DirectionLight, u_Material);
	result += CalculateSpotLight(u_SpotLight, u_Material);
	FragColor = vec4(result, 1.0f);
} 