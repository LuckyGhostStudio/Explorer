#version 330 core
			
layout(location = 0) out vec4 color;	//片元输出颜色

in vec3 v_FragPos;	//片元位置
in vec4 v_Color;	//颜色
in vec3 v_Normal;	//法线

uniform vec3 u_AmbientColor;	//环境光颜色 
uniform vec3 u_LightColor;		//灯光颜色
uniform vec3 u_LightPos;		//灯光位置
uniform vec3 u_CameraPos;		//相机位置
			
void main()
{
	vec3 cameraDir = normalize(u_CameraPos - v_FragPos);	//指向相机方向
	vec3 lightDir = normalize(u_LightPos - v_FragPos);		//指向灯光方向

	//漫反射颜色
	float diffuseIntensity = max(dot(lightDir, v_Normal), 0);	//漫反射强度 [0, 1]
	vec3 diffuseColor =  diffuseIntensity * u_LightColor;		//漫反射颜色 = 漫反射强度 * 灯光颜色

	//镜面反射颜色
	vec3 reflectDir = reflect(-lightDir, v_Normal);								//反射光方向
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), 32.0f);	//镜面反射强度 [0, 1]		反光度32
	vec3 specularColor = specularIntensity * u_LightColor;						//镜面反射颜色 =  镜面反射强度 * 灯光颜色

	vec3 ambientColor = u_AmbientColor * v_Color.rgb;

	vec3 resultColor = v_Color.rgb * (u_AmbientColor + diffuseColor + specularColor);

	color = vec4(resultColor, 1.0f);
}