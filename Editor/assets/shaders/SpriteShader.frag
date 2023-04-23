#version 450 core
			
layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ
layout(location = 1) out int objectID;	//��ɫ������1ƬԪ���ֵ������id

in vec4 v_Color;
in vec3 v_Normal;
in vec2 v_TexCoord;
in flat int v_ID;
in flat int v_ObjectID;

uniform sampler2D u_Texture;	//Sprite����
uniform bool u_TextureExist;
			
void main()
{
	color = v_Color;

	if(u_TextureExist){
		color *= texture(u_Texture, v_TexCoord);
	}
	
	objectID = v_ObjectID;
}