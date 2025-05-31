#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture1;
uniform sampler2D u_Texture2;
uniform float u_TextureMixLinear;

void main()
{
	FragColor = mix(texture(u_Texture1, TexCoord), texture(u_Texture2, TexCoord), u_TextureMixLinear) * vec4(u_Color);
} 