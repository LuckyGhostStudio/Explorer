#version 450 core
			
layout(location = 0) out vec4 color;	//颜色缓冲区0片元输出颜色
layout(location = 1) out int objectID;	//颜色缓冲区1片元输出值：物体id

in vec3 v_FragPos;		//片元位置
in vec4 v_Color;		//颜色
in vec3 v_Normal;		//法线
in vec2 v_TexCoord;		//纹理坐标
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

//材质
struct Material
{
	vec4 Color;					//颜色

	sampler2D AlbedoTexture;	//反照率贴图
	sampler2D SpecularTexture;	//高光贴图
	
	bool AlbedoTextureExist;	//反照率贴图存在
	bool SpecularTextureExist;	//高光贴图存在

	float Shininess;			//反光度

	vec2 Tiling;				//纹理重复因子
	vec2 Offset;				//纹理偏移量
};

struct Environment
{
	bool EnableSkybox;			//是否启用天空盒
	samplerCube Cubemap;		//立方体贴图（Skybox）
	mat4 SkyboxRotationMatrix;	//Skybox的旋转矩阵

	int SourceType;				//环境光源类型：0 天空盒 1 颜色
	float IntensityMultiplier;	//Skybox光强倍数
	vec3 AmbientColor;			//环境光颜色
};

uniform int u_LightCount = 1;	//当前光源数量
uniform Light u_Lights[24];		//场景所有光源

uniform bool u_MaterialExist = false;	//材质是否存在
uniform Material u_Material;			//材质

uniform Environment u_Environment;	//环境设置

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

//计算冯氏光照模型光照
vec3 CalculatePhongLighting(vec3 lightDir, vec3 lightColor, vec3 cameraDir)
{
	//漫反射颜色
	float albedoIntensity = max(dot(lightDir, v_Normal), 0);				//漫反射强度 [0, 1]
	vec3 albedoTexColor;													//漫发射贴图颜色
	vec3 albedoColor = albedoIntensity * lightColor;						//漫反射颜色 = 漫反射强度 * 灯光颜色 
	//漫反射贴图存在
	if(u_Material.AlbedoTextureExist){
		albedoTexColor = texture(u_Material.AlbedoTexture, (v_TexCoord + u_Material.Offset) * u_Material.Tiling).rgb;	//取漫反射贴图在v_TexCoord坐标的rgb值 （纹理坐标 + 偏移量）* 平铺因子
		albedoColor *= albedoTexColor;										//漫反射颜色 = 累积 漫反射贴图颜色
	}

	//镜面反射颜色
	vec3 reflectDir = reflect(-lightDir, v_Normal);												//反射光方向
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), u_Material.Shininess);	//镜面反射强度 [0, 1]	 反光度
	vec3 specularTexColor;
	vec3 specularColor = specularIntensity * lightColor;										//镜面反射颜色 = 镜面反射强度 * 灯光颜色
	//镜面反射贴图存在
	if(u_Material.SpecularTextureExist){
		specularTexColor = texture(u_Material.SpecularTexture, (v_TexCoord + u_Material.Offset) * u_Material.Tiling).rgb;	//取镜面反射贴图在v_TexCoord坐标的rgb值
		specularColor *= specularTexColor;													//镜面反射颜色 累积 镜面反射贴图颜色
	}

	//环境光颜色
	vec3 ambientColor = albedoTexColor * u_Environment.AmbientColor;

	return albedoColor + specularColor + ambientColor;	//累加 漫反射光 镜面反射光 环境光
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
	vec4 objectColor = v_Color;						//物体颜色 = 顶点颜色（材质缺失颜色）

	//材质存在
	if(u_MaterialExist){
		objectColor = u_Material.Color;	//物体颜色 = 材质颜色

		vec3 cameraDir = normalize(u_CameraPos - v_FragPos);	//指向相机方向
		vec3 sightReflectDir = reflect(-cameraDir, v_Normal);	//目光反射方向（Skybox光照映射采样方向）：物体反射Skybox颜色
		vec3 beamColor;	//光照颜色

		//天空盒已启用：计算Skybox映射
		if(u_Environment.EnableSkybox){
			sightReflectDir = (u_Environment.SkyboxRotationMatrix * vec4(sightReflectDir, 1.0)).xyz;	//对反射光应用天空盒旋转变换
			sightReflectDir = vec3(sightReflectDir.x, -sightReflectDir.yz);	//根据Cubemap翻转方式 对反射方向做变换（Cubemap yz轴颠倒）
			//反射的Skybox颜色：使用sightReflectDir从Cubemap采样 叠加 Skybox光强度倍数
			vec3 skyboxColor = texture(u_Environment.Cubemap, sightReflectDir).rgb * (0.5 + u_Environment.IntensityMultiplier);
			beamColor = skyboxColor;	//光照颜色 = Skybox映射颜色

			//环境光源类型为 Color：叠加环境光源类型为颜色的环境光颜色
			if(u_Environment.SourceType == 1){
				beamColor *= u_Environment.AmbientColor;	//光照颜色 叠加 环境光颜色
			}
		}
		else{//天空盒未启用：添加环境光源类型为颜色的环境光颜色
			beamColor = u_Environment.AmbientColor;		//光照颜色 = 环境光颜色
		}

		//遍历所有Light：计算灯光光照
		for(int i = 0; i < u_LightCount; i++){
			switch(u_Lights[i].Type){	//光照类型
				case 0:	//平行光
					beamColor += CalculateDirectionalLight(u_Lights[i], cameraDir);	//计算平行光照
					break;
				case 1:	//点光源
					beamColor += CalculatePointLight(u_Lights[i], cameraDir);		//计算点光源光照
					break;
				case 2:	//聚光源
					beamColor += CalculateSpotLight(u_Lights[i], cameraDir);		//计算点聚光源光照
					break;
			}
		}

		objectColor *= vec4(beamColor, 1.0f);	//物体颜色 叠加 灯光颜色
	}

	color = objectColor;	//颜色缓冲区0输出颜色

	objectID = v_ObjectID;
}