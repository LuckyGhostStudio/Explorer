#version 450 core

layout(location = 0) in vec3 a_Position;	//λ��
layout(location = 1) in vec4 a_Color;		//��ɫ
layout(location = 2) in vec3 a_Normal;		//����
layout(location = 3) in vec2 a_TexCoord;	//��������
//layout(location = 4) in float a_TexIndex;	//��������
layout(location = 4) in int a_ID;			//����ID
layout(location = 5) in int a_ObjectID;		//����ID

uniform mat4 u_ViewProjectionMatrix;	//vp���� p * v

out vec3 v_FragPos;		//ƬԪλ��
out vec4 v_Color;		//��ɫ
out vec3 v_Normal;		//����
out vec2 v_TexCoord;	//��������
//out float v_TexIndex;	//��������
out flat int v_ID;		//����ID
out flat int v_ObjectID;//����ID

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);	//����λ�ã���Ļ���꣩
	
	v_FragPos = a_Position;				//ƬԪλ��Ϊ����ʵ��λ�ã��ռ����꣩
	v_Color = a_Color;
	v_Normal = normalize(a_Normal);		//��������һ��
	v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
	v_ID = a_ID;
	v_ObjectID = a_ObjectID;
}