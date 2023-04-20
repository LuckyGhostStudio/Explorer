#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;
in flat int v_ObjectID;

uniform vec4 u_Color;
uniform sampler2D u_Textures[32];	//纹理 0-31
			
void main()
{
	//color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * u_Color;
	color = v_Color;

	objectID = v_ObjectID;
}