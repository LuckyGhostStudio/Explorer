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

//光源
struct Light
{
	vec3 Position;		//位置
	vec3 Direction;		//光照方向 z-

	vec3 Color;			//光照颜色
	float Intensity;	//光照强度：TODO:光照强度对颜色的影响
	int RenderShadow;	//是否渲染阴影 0不渲染 1渲染 --- TODO:添加阴影渲染

	int Type;				//光照类型
	float Range;			//光照半径 Point | Spot：光照强度衰减为0时的距离
	float SpotOuterAngle;	//外张角 弧度 Spot
	float SpotInnerAngle;	//内张角 弧度 Spot
};

uniform int u_LightCount = 1;	//当前光源数量
uniform Light u_Lights[24];		//场景所有光源

uniform vec3 u_AmbientColor;	//环境光颜色
uniform vec3 u_CameraPos;		//相机位置

//计算光源强度衰减系数
float CalculateAttenuation(Light light)
{
	float constant = 1.0f;		//灯光强度衰减常数项系数（Point和Spot）
	float linear = 0.0f;		//灯光强度衰减一次项系数（Point和Spot）
	float quadratic = 0.0f;		//灯光强度衰减二次项系数（Point和Spot）

	float range = light.Range;	//光源范围

	if(range == 0.0f){ 
		return 0.0f;	//光源范围为0 衰减系数为0
	}
	else if(range > 0.0f && range <= 65.0f){
		linear = 4.9786f * pow(range, -1.028f);		//linear = f1(range)
		quadratic = 93.918f * pow(range, -2.061f);	//quadratic = g1(range)
	}
	else if(range > 65.0f && range <= 600.0f){
		linear = 5.0514f * pow(range, -1.026f);		//linear = f2(range)
		quadratic = 73.744f * pow(range, -2.001f);	//quadratic = g2(range)
	}
	else{
		linear = 3.1019f * pow(range, -0.953f);		//linear = f3(range)
		quadratic = 65.113f * pow(range, -1.984f);	//quadratic = g3(range)
	}

	float dist = length(light.Position - v_FragPos);									//光源距片元的距离
	float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));	//光源强度衰减系数

	return attenuation;
}

//计算冯氏光照模型光照：漫反射光 + 镜面反射光 TODO:增加材质后添加光照贴图 和 反照率参数
vec3 CalculatePhongLighting(vec3 lightDir, vec3 lightColor, vec3 cameraDir)
{
	//漫反射颜色
	float diffuseIntensity = max(dot(lightDir, v_Normal), 0);	//漫反射强度 [0, 1]
	vec3 diffuseColor =  diffuseIntensity * lightColor;			//漫反射颜色 = 漫反射强度 * 灯光颜色 

	//镜面反射颜色
	vec3 reflectDir = reflect(-lightDir, v_Normal);								//反射光方向
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), 32.0f);	//镜面反射强度 [0, 1]		反光度32
	vec3 specularColor = specularIntensity * lightColor;						//镜面反射颜色 = 镜面反射强度 * 灯光颜色

	return diffuseColor + specularColor;	//累加 漫反射光 镜面反射光
}

//计算平行光照
vec3 CalculateDirectionalLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(-light.Direction);		//指向光源
	vec3 lightColor = light.Color * light.Intensity;	//光照颜色 = 光源颜色 * 光照强度

	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//计算冯氏光照模型光照：漫反射光 + 镜面反射光
}

//计算点光源光照
vec3 CalculatePointLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(light.Position - v_FragPos);								//片元指向光源
	vec3 lightColor = CalculateAttenuation(light) * light.Color * light.Intensity;	//衰减后的灯光颜色 = 衰减系数 * 光源颜色 * 光照强度
	
	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//计算冯氏光照模型光照：漫反射光 + 镜面反射光
}

//计算聚光源光照
vec3 CalculateSpotLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(light.Position - v_FragPos);								//片元指向光源
	vec3 lightColor = CalculateAttenuation(light) * light.Color * light.Intensity;	//衰减后的灯光颜色 = 衰减系数 * 光源颜色 * 光照强度

	//计算聚光效果
	float cosTheta = dot(-lightDir, light.Direction);	//指向该片元的光线 与 SpotLight方向 夹角的 cos
	float spotRatio;									//聚光比率
	float cosAngleInner = cos(light.SpotInnerAngle);	//内张角余弦
	float cosAngleOutter = cos(light.SpotOuterAngle);	//外张角余弦

	if(cosTheta > cosAngleInner){	//在SpotLight内张角内	 内
		spotRatio = 1.0f;
	}
	else if(cosTheta > cosAngleOutter){	//在外张角内 中
		spotRatio = (cosTheta - cosAngleOutter) / (cosAngleInner - cosAngleOutter);		//从inner到outter线性减小
	}
	else{	//在外张角外 外
		spotRatio = 0.0f;
	}

	lightColor = lightColor * spotRatio;		//计算聚光后的颜色

	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//计算冯氏光照模型光照：漫反射光 + 镜面反射光
}

void main()
{
	vec3 lightColor = u_AmbientColor;	//光照颜色 初值 = 环境光颜色

	vec3 cameraDir = normalize(u_CameraPos - v_FragPos);	//指向相机方向
	
	//遍历所有Light
	for(int i = 0; i < u_LightCount; i++){
		switch(u_Lights[i].Type){	//光照类型
			case 0:	//平行光
				lightColor += CalculateDirectionalLight(u_Lights[i], cameraDir);	//计算平行光照
				break;
			case 1:	//点光源
				lightColor += CalculatePointLight(u_Lights[i], cameraDir);	//计算点光源光照
				break;
			case 2:	//聚光源
				lightColor += CalculateSpotLight(u_Lights[i], cameraDir);	//计算点聚光源光照
				break;
		}
	}

	color = vec4(v_Color.rgb * lightColor, 1.0f);	//颜色缓冲区0输出颜色

	objectID = v_ObjectID;
}