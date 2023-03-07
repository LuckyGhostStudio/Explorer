#version 330 core
			
layout(location = 0) out vec4 color;	//片元输出颜色

in vec3 v_FragPos;	//片元位置
in vec4 v_Color;	//颜色
in vec3 v_Normal;	//法线

uniform vec3 u_AmbientColor;	//环境光颜色 
uniform vec3 u_LightColor;		//灯光颜色
uniform vec3 u_LightDir;		//灯光照射方向
uniform vec3 u_CameraPos;		//相机位置
			
void main()
{
	vec3 cameraDir = normalize(u_CameraPos - v_FragPos);		//指向相机方向

	//漫反射颜色
	float diffuseIntensity = max(dot(-u_LightDir, v_Normal), 0);	//漫反射强度 [0, 1]
	vec3 diffuseColor =  diffuseIntensity * u_LightColor;			//漫反射颜色 = 漫反射强度 * 灯光颜色

	//镜面反射颜色
	vec3 reflectDir = reflect(u_LightDir, v_Normal);							//反射光方向
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), 32.0f);	//镜面反射强度 [0, 1]
	vec3 specularColor = specularIntensity * u_LightColor;						//镜面反射颜色 =  镜面反射强度 * 灯光颜色

	vec3 ambientColor = u_AmbientColor * v_Color.rgb;

	color = v_Color * vec4(u_AmbientColor + diffuseColor + specularColor, 1.0f);
}