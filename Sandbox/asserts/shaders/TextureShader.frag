#version 330 core
			
layout(location = 0) out vec4 color;	//输出颜色

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in vec2 v_TilingFactor;

uniform vec2 u_TilingFactor;		//纹理重复因子
uniform sampler2D u_Textures[32];	//纹理 0-31
			
void main()
{
	color = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}