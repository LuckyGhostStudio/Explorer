#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

in vec3 v_FragPos;		//片元位置
in vec4 v_Color;		//颜色
in vec3 v_Normal;		//法线
//in vec2 v_TexCoord;		//纹理坐标
//in float v_TexIndex;	//纹理索引
in flat int v_ID;			//顶点ID
in flat int v_ObjectID;		//物体ID

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
	vec3 specularColor = specularIntensity * u_LightColor;						//镜面反射颜色 = 镜面反射强度 * 灯光颜色

	vec3 ambientColor = u_AmbientColor * v_Color.rgb;

	vec3 resultColor = v_Color.rgb * (u_AmbientColor + diffuseColor + specularColor);

	color = vec4(resultColor, 1.0f);

	objectID = v_ObjectID;
}