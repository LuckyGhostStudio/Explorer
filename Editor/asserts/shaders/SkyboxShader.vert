#version 450 core

layout (location = 0) in vec3 a_Position;   //天空盒顶点位置

uniform mat4 u_RotationMatrix;     //旋转矩阵
uniform mat4 u_ViewMatrix;	        //view矩阵
uniform mat4 u_ProjectionMatrix;	//projection矩阵

out vec3 v_TexCoord;  //采样方向向量（立方体中心指向立方体表面像素）

void main()
{
    vec4 position = u_RotationMatrix * vec4(a_Position, 1.0);   //做旋转变换
    v_TexCoord = vec3(a_Position.x, -a_Position.yz);            //纹理采样 翻转Cubemap yz轴

    position = u_ProjectionMatrix * u_ViewMatrix * position;
    gl_Position = position.xyww;    //变换后做透视除法 xyz/w 使 z = 1.0
    
}