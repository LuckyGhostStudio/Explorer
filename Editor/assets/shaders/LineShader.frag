#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

in vec4 v_Color;
in flat int v_ObjectID;
			
void main()
{
	color = v_Color;
	
	objectID = v_ObjectID;
}