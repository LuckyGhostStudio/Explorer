#version 450 core
			
layout(location = 0) in vec3 a_Position;	//位置
layout(location = 1) in vec4 a_Color;		//颜色
layout(location = 2) in vec2 a_TexCoord;	//纹理坐标
layout(location = 3) in float a_TexIndex;	//纹理索引
layout(location = 4) in int a_ObjectID;		//所属物体ID

uniform mat4 u_ViewProjectionMatrix;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;
out flat int v_ObjectID;

void main()
{
	v_Color = a_Color;
	v_TexCoord = a_TexCoord;
	v_ObjectID = a_ObjectID;

	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
}