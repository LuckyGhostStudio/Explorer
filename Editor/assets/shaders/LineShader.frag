#version 450 core
			
layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ
layout(location = 1) out int objectID;	//��ɫ������1ƬԪ���ֵ������id

in vec4 v_Color;
in flat int v_ObjectID;
			
void main()
{
	color = v_Color;
	
	objectID = v_ObjectID;
}