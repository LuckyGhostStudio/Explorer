#version 450 core
			
layout(location = 0) in vec3 a_Position;	//λ��
layout(location = 1) in vec4 a_Color;		//��ɫ
layout(location = 2) in int a_ObjectID;		//����ID

uniform mat4 u_ViewProjectionMatrix;

out vec4 v_Color;
out flat int v_ObjectID;

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
	
	v_Color = a_Color;
	v_ObjectID = a_ObjectID;
}