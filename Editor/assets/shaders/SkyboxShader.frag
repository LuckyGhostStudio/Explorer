#version 450 core

layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色

in vec3 v_TexCoord;

uniform samplerCube u_Cubemap;
uniform vec3 u_TintColor;   //天空盒色调
uniform float u_Expose;     //天空盒曝光度

void main()
{    
    vec4 skyboxColor = textureCube(u_Cubemap, v_TexCoord);  //天空盒实际颜色：根据采样方向向量在Cubemap上采样
    color = skyboxColor * (1.0 + u_Expose) * vec4(u_TintColor, 1.0);   //叠加 曝光度 和 色调
}