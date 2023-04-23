#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_TexCoord;
in flat int v_ID;
in flat int v_ObjectID;

uniform sampler2D u_Texture;	//Sprite纹理
uniform bool u_TextureExist;
			
void main()
{
	color = v_Color;

	if(u_TextureExist){
		color *= texture(u_Texture, v_TexCoord);
	}
	
	objectID = v_ObjectID;
}