#version 330 core
out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 u_Color;
uniform vec3 u_LightColor;

uniform float u_AmbientStrength;
uniform vec3 u_LightPos;
uniform vec3 u_CameraPos;
uniform float u_SpecularStrength;

void main()
{
	vec3 ambient = vec3(u_AmbientStrength * u_LightColor);

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * u_LightColor;

	vec3 viewDir = normalize(u_CameraPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = u_SpecularStrength * pow(max(dot(viewDir, reflectDir), 0.0), 256) * u_LightColor;

	vec3 result = (ambient + diffuse + specular) * u_Color;
	FragColor = vec4(result, 1.0f);
} 