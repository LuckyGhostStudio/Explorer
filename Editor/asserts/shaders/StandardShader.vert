#version 450 core

layout(location = 0) in vec3 a_Position;	//位置
layout(location = 1) in vec4 a_Color;		//颜色
layout(location = 2) in vec3 a_Normal;		//法线
layout(location = 3) in vec2 a_TexCoord;	//纹理坐标
//layout(location = 4) in float a_TexIndex;	//纹理索引
layout(location = 4) in int a_ID;			//顶点ID
layout(location = 5) in int a_ObjectID;		//物体ID

uniform mat4 u_ViewProjectionMatrix;	//vp矩阵 p * v

out vec3 v_FragPos;		//片元位置
out vec4 v_Color;		//颜色
out vec3 v_Normal;		//法线
out vec2 v_TexCoord;	//纹理坐标
//out float v_TexIndex;	//纹理索引
out flat int v_ID;		//顶点ID
out flat int v_ObjectID;//物体ID

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);	//顶点位置（屏幕坐标）
	
	v_FragPos = a_Position;				//片元位置为顶点实际位置（空间坐标）
	v_Color = a_Color;
	v_Normal = normalize(a_Normal);		//法向量归一化
	v_TexCoord = a_TexCoord;
	//v_TexIndex = a_TexIndex;
	v_ID = a_ID;
	v_ObjectID = a_ObjectID;
}