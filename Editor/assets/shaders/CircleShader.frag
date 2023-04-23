#version 450 core
			
layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ
layout(location = 1) out int objectID;	//��ɫ������1ƬԪ���ֵ������id

in vec3 v_LocalPosition;
in vec4 v_Color;
in float v_Thickness;
in float v_Fade;
in flat int v_ObjectID;
			
void main()
{
    float dist = 1.0 - length(v_LocalPosition);
    float circle = smoothstep(0.0, v_Fade, dist);
    circle *= smoothstep(v_Thickness + v_Fade, v_Thickness, dist);

	//����ԲȦ�� ����
	if (circle == 0.0)
		discard;

	color = v_Color;
	color.a *= circle;

	objectID = v_ObjectID;
}