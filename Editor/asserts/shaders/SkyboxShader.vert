#version 450 core

layout (location = 0) in vec3 a_Position;   //��պж���λ��

uniform mat4 u_RotationMatrix;     //��ת����
uniform mat4 u_ViewMatrix;	        //view����
uniform mat4 u_ProjectionMatrix;	//projection����

out vec3 v_TexCoord;  //������������������������ָ��������������أ�

void main()
{
    vec4 position = u_RotationMatrix * vec4(a_Position, 1.0);   //����ת�任
    v_TexCoord = vec3(a_Position.x, -a_Position.yz);            //������� ��תCubemap yz��

    position = u_ProjectionMatrix * u_ViewMatrix * position;
    gl_Position = position.xyww;    //�任����͸�ӳ��� xyz/w ʹ z = 1.0
    
}