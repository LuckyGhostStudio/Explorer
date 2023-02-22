#version 330 core
			
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

out vec3 v_Position;						

void main()
{
	v_Position = a_Position;
	gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
}