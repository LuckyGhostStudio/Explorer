#version 450 core
			
layout(location = 0) in vec3 a_Position;

out vec2 v_ScreenCoord;

void main()
{
    v_ScreenCoord = (vec2(a_Position.x, a_Position.y) + 1.0) / 2.0; //×ª»»µ½[0, 1]
    gl_Position = vec4(a_Position, 1.0f);
}