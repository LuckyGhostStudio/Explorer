#version 330

layout(location = 0) in vec3 a_Position;	//λ��
layout(location = 1) in vec4 a_Color;		//��ɫ
layout(location = 2) in vec3 a_Normal;		//����

uniform mat4 u_ViewProjectionMatrix;	//vp���� p * v
uniform mat4 u_Transform;				//ģ�ͱ任����

out vec3 v_FragPos;	//ƬԪλ��
out vec4 v_Color;
out vec3 v_Normal;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);	//����λ�ã���Ļ���꣩
	v_FragPos = (u_Transform * vec4(a_Position, 1.0)).xyz;						//ƬԪλ��Ϊ����ʵ��λ�ã��ռ����꣩
	v_Color = a_Color;
	v_Normal = normalize(mat3(transpose(inverse(u_Transform))) * a_Normal);	//�������� M �任 Mȡ������ת�� ��ֹnormal������ʱ������
}