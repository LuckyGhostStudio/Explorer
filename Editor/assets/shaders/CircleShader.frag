#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

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

	//超出圆圈外 丢弃
	if (circle == 0.0)
		discard;

	color = v_Color;
	color.a *= circle;

	objectID = v_ObjectID;
}