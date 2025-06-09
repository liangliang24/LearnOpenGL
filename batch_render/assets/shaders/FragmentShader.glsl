#version 450 core
layout (location = 0) out vec4 o_Color;
in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIdx;

uniform sampler2D u_Textures[2];

void main()
{
	int texIdx = int(v_TexIdx + 0.5f);
	o_Color = texture(u_Textures[texIdx], v_TexCoord);
	//o_Color = vec4(texIdx, texIdx, texIdx, 1.0f);
} 