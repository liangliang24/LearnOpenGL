#version 450 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec4 a_Color;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in float a_TexIdx;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIdx;

uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Pos, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIdx = a_TexIdx;
}