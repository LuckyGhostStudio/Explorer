#version 450 core

layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ

in vec3 v_TexCoord;

uniform samplerCube u_Cubemap;

void main()
{    
    color = textureCube(u_Cubemap, v_TexCoord); //���ݲ�������������Cubemap�ϲ���
}