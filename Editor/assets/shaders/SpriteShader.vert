#version 450 core
			
layout(location = 0) in vec3 a_Position;	//位置
layout(location = 1) in vec4 a_Color;		//颜色
layout(location = 2) in vec3 a_Normal;		//法线
layout(location = 3) in vec2 a_TexCoord;	//纹理坐标
layout(location = 4) in int a_ID;			//顶点ID
layout(location = 5) in int a_ObjectID;		//物体ID

uniform mat4 u_ViewProjectionMatrix;

out vec4 v_Color;
out vec3 v_Normal;
out vec2 v_TexCoord;
out flat int v_ID;
out flat int v_ObjectID;

void main()
{
	gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
	
	v_Color = a_Color;
	v_Normal = a_Normal;
	v_TexCoord = a_TexCoord;
	v_ID = a_ID;
	v_ObjectID = a_ObjectID;
}