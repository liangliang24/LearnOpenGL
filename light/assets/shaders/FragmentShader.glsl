#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec4 u_Color;
uniform vec4 u_LightColor;

void main()
{
	FragColor = vec4(u_LightColor * u_Color);
} 