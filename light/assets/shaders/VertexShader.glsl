#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoords;

uniform mat4 u_Transform;
uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main()
{
    gl_Position = u_Projection * u_View * u_Transform * vec4(a_Pos, 1.0);
    FragPos = vec3(u_Transform * vec4(a_Pos, 1.0f));
    Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
    TexCoords = a_TexCoords;
}