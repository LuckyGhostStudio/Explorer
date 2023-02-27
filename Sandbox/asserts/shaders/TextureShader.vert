#version 330 core
			
layout(location = 0) in vec3 a_Position;		//位置
layout(location = 1) in vec4 a_Color;			//颜色
layout(location = 2) in vec2 a_TexCoord;		//纹理坐标
layout(location = 3) in float a_TexIndex;		//纹理索引
layout(location = 4) in vec2 a_TilingFactor;	//纹理平铺因子

uniform mat4 u_ViewProjectionMatrix;
//uniform mat4 u_Transform;

out vec4 v_Color;
out vec2 v_TexCoord;	
out float v_TexIndex;
out vec2 v_TilingFactor;

void main()
{
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
	v_TilingFactor = a_TilingFactor;
	gl_Position = u_ViewProjectionMatrix * /*u_Transform **/ vec4(a_Position, 1.0);
}