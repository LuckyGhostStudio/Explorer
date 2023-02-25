#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;			//颜色
uniform vec2 u_TilingFactor;	//纹理重复因子
uniform sampler2D u_Texture;	//纹理
			
void main()
{
	color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}