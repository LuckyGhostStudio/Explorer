#version 330

layout(location = 0) in vec3 a_Position;	//位置
layout(location = 1) in vec4 a_Color;		//颜色
layout(location = 2) in vec3 a_Normal;		//法线

uniform mat4 u_ViewProjectionMatrix;	//vp矩阵 p * v
uniform mat4 u_Transform;				//模型变换矩阵

out vec3 v_FragPos;	//片元位置
out vec4 v_Color;
out vec3 v_Normal;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);	//顶点位置（屏幕坐标）
	v_FragPos = (u_Transform * vec4(a_Position, 1.0)).xyz;						//片元位置为顶点实际位置（空间坐标）
	v_Color = a_Color;
	v_Normal = normalize(mat3(transpose(inverse(u_Transform))) * a_Normal);	//法向量做 M 变换 M取逆矩阵的转置 防止normal在缩放时被拉伸
}