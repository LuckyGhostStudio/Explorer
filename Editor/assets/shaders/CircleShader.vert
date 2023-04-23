#version 450 core
			
layout(location = 0) in vec3 a_WorldPosition;	//����λ��
layout(location = 1) in vec3 a_LocalPosition;	//����λ��
layout(location = 2) in vec4 a_Color;			//��ɫ
layout(location = 3) in float a_Thickness;		//���
layout(location = 4) in float a_Fade;			//ģ����
layout(location = 5) in int a_ObjectID;			//����ID

uniform mat4 u_ViewProjectionMatrix;

out vec3 v_LocalPosition;
out vec4 v_Color;
out float v_Thickness;
out float v_Fade;
out flat int v_ObjectID;

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_WorldPosition, 1.0);
	
	v_LocalPosition = a_LocalPosition;
	v_Color = a_Color;
	v_Thickness = a_Thickness;
	v_Fade = a_Fade;
	v_ObjectID = a_ObjectID;
}