#version 450 core

layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ

in vec3 v_TexCoord;

uniform samplerCube u_Cubemap;
uniform vec3 u_TintColor;   //��պ�ɫ��
uniform float u_Expose;     //��պ��ع��

void main()
{    
    vec4 skyboxColor = textureCube(u_Cubemap, v_TexCoord);  //��պ�ʵ����ɫ�����ݲ�������������Cubemap�ϲ���
    color = skyboxColor * (1.0 + u_Expose) * vec4(u_TintColor, 1.0);   //���� �ع�� �� ɫ��
}