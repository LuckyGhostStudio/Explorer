#version 450 core

layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色

in vec3 v_TexCoord;

uniform samplerCube u_Cubemap;

void main()
{    
    color = textureCube(u_Cubemap, v_TexCoord); //根据采样方向向量在Cubemap上采样
}