#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 u_Color;
uniform vec3 u_LightColor;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int shininess;
};

uniform Material u_Material;

struct Light
{
	vec3 position;

	float ambient;
	float diffuse;
	float specular;
};

uniform Light u_Light;

uniform vec3 u_CameraPos;

void main()
{
	vec3 ambient = u_Light.ambient * u_LightColor * u_Material.ambient;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_Light.position - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * u_Material.diffuse * u_LightColor * u_Light.diffuse;

	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	vec3 specular = pow(max(dot(norm, halfwayDir), 0.0), u_Material.shininess) * u_Material.specular * u_LightColor * u_Light.specular;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
} 