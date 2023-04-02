#version 450 core
			
layout(location = 0) out vec4 color;	//��ɫ������0ƬԪ�����ɫ
layout(location = 1) out int objectID;	//��ɫ������1ƬԪ���ֵ������id

in vec3 v_FragPos;		//ƬԪλ��
in vec4 v_Color;		//��ɫ
in vec3 v_Normal;		//����
in vec2 v_TexCoord;		//��������
//in float v_TexIndex;	//��������
in flat int v_ID;			//����ID
in flat int v_ObjectID;		//����ID

//��Դ
struct Light
{
	vec3 Position;		//λ��
	vec3 Direction;		//���շ��� z-

	vec3 Color;			//������ɫ
	float Intensity;	//����ǿ�ȣ�TODO:����ǿ�ȶ���ɫ��Ӱ��
	int RenderShadow;	//�Ƿ���Ⱦ��Ӱ 0����Ⱦ 1��Ⱦ --- TODO:�����Ӱ��Ⱦ

	int Type;				//��������
	float Range;			//���հ뾶 Point | Spot������ǿ��˥��Ϊ0ʱ�ľ���
	float SpotOuterAngle;	//���Ž� ���� Spot
	float SpotInnerAngle;	//���Ž� ���� Spot
};

//����
struct Material
{
	vec4 Color;					//��ɫ

	sampler2D AlbedoTexture;	//��������ͼ
	sampler2D SpecularTexture;	//�߹���ͼ
	
	bool AlbedoTextureExist;	//��������ͼ����
	bool SpecularTextureExist;	//�߹���ͼ����

	float Shininess;			//�����

	vec2 Tiling;				//�����ظ�����
	vec2 Offset;				//����ƫ����
};

uniform int u_LightCount = 1;	//��ǰ��Դ����
uniform Light u_Lights[24];		//�������й�Դ

uniform bool u_MaterialExist = false;	//�����Ƿ����
uniform Material u_Material;			//����

uniform vec3 u_AmbientColor;	//��������ɫ
uniform vec3 u_CameraPos;		//���λ��

//�����Դǿ��˥��ϵ��
float CalculateAttenuation(Light light)
{
	float constant = 1.0f;		//�ƹ�ǿ��˥��������ϵ����Point��Spot��
	float linear = 0.0f;		//�ƹ�ǿ��˥��һ����ϵ����Point��Spot��
	float quadratic = 0.0f;		//�ƹ�ǿ��˥��������ϵ����Point��Spot��

	float range = light.Range;	//��Դ��Χ

	if(range == 0.0f){ 
		return 0.0f;	//��Դ��ΧΪ0 ˥��ϵ��Ϊ0
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

	float dist = length(light.Position - v_FragPos);									//��Դ��ƬԪ�ľ���
	float attenuation = 1.0f / (constant + linear * dist + quadratic * (dist * dist));	//��Դǿ��˥��ϵ��

	return attenuation;
}

//������Ϲ���ģ�͹���
vec3 CalculatePhongLighting(vec3 lightDir, vec3 lightColor, vec3 cameraDir)
{
	//��������ɫ
	float albedoIntensity = max(dot(lightDir, v_Normal), 0);				//������ǿ�� [0, 1]
	vec3 albedoTexColor;													//��������ͼ��ɫ
	vec3 albedoColor = albedoIntensity * lightColor;						//��������ɫ = ������ǿ�� * �ƹ���ɫ 
	//��������ͼ����
	if(u_Material.AlbedoTextureExist){
		albedoTexColor = texture(u_Material.AlbedoTexture, (v_TexCoord + u_Material.Offset) * u_Material.Tiling).rgb;	//ȡ��������ͼ��v_TexCoord�����rgbֵ ���������� + ƫ������* ƽ������
		albedoColor *= albedoTexColor;										//��������ɫ = �ۻ� ��������ͼ��ɫ
	}

	//���淴����ɫ
	vec3 reflectDir = reflect(-lightDir, v_Normal);												//����ⷽ��
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), u_Material.Shininess);	//���淴��ǿ�� [0, 1]	 �����
	vec3 specularTexColor;
	vec3 specularColor = specularIntensity * lightColor;										//���淴����ɫ = ���淴��ǿ�� * �ƹ���ɫ
	//���淴����ͼ����
	if(u_Material.SpecularTextureExist){
		specularTexColor = texture(u_Material.SpecularTexture, (v_TexCoord + u_Material.Offset) * u_Material.Tiling).rgb;	//ȡ���淴����ͼ��v_TexCoord�����rgbֵ
		specularColor *= specularTexColor;													//���淴����ɫ �ۻ� ���淴����ͼ��ɫ
	}

	//��������ɫ
	vec3 ambientColor = albedoTexColor * u_AmbientColor;

	return albedoColor + specularColor + ambientColor;	//�ۼ� ������� ���淴��� ������
}

//����ƽ�й���
vec3 CalculateDirectionalLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(-light.Direction);		//ָ���Դ
	vec3 lightColor = light.Color * light.Intensity;	//������ɫ = ��Դ��ɫ * ����ǿ��

	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//������Ϲ���ģ�͹��գ�������� + ���淴���
}

//������Դ����
vec3 CalculatePointLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(light.Position - v_FragPos);								//ƬԪָ���Դ
	vec3 lightColor = CalculateAttenuation(light) * light.Color * light.Intensity;	//˥����ĵƹ���ɫ = ˥��ϵ�� * ��Դ��ɫ * ����ǿ��
	
	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//������Ϲ���ģ�͹��գ�������� + ���淴���
}

//����۹�Դ����
vec3 CalculateSpotLight(Light light, vec3 cameraDir)
{
	vec3 lightDir = normalize(light.Position - v_FragPos);								//ƬԪָ���Դ
	vec3 lightColor = CalculateAttenuation(light) * light.Color * light.Intensity;	//˥����ĵƹ���ɫ = ˥��ϵ�� * ��Դ��ɫ * ����ǿ��

	//����۹�Ч��
	float cosTheta = dot(-lightDir, light.Direction);	//ָ���ƬԪ�Ĺ��� �� SpotLight���� �нǵ� cos
	float spotRatio;									//�۹����
	float cosAngleInner = cos(light.SpotInnerAngle);	//���Ž�����
	float cosAngleOutter = cos(light.SpotOuterAngle);	//���Ž�����

	if(cosTheta > cosAngleInner){	//��SpotLight���Ž���	 ��
		spotRatio = 1.0f;
	}
	else if(cosTheta > cosAngleOutter){	//�����Ž��� ��
		spotRatio = (cosTheta - cosAngleOutter) / (cosAngleInner - cosAngleOutter);		//��inner��outter���Լ�С
	}
	else{	//�����Ž��� ��
		spotRatio = 0.0f;
	}

	lightColor = lightColor * spotRatio;		//����۹�����ɫ

	return CalculatePhongLighting(lightDir, lightColor, cameraDir);	//������Ϲ���ģ�͹��գ�������� + ���淴���
}

void main()
{
	vec4 objectColor = v_Color;			//������ɫ = ������ɫ������ȱʧ��ɫ��
	vec3 lightColor = u_AmbientColor;	//������ɫ ��ֵ = ��������ɫ

	//���ʴ���
	if(u_MaterialExist){
		vec3 cameraDir = normalize(u_CameraPos - v_FragPos);	//ָ���������
		//��������Light���������
		for(int i = 0; i < u_LightCount; i++){
			switch(u_Lights[i].Type){	//��������
				case 0:	//ƽ�й�
					lightColor += CalculateDirectionalLight(u_Lights[i], cameraDir);	//����ƽ�й���
					break;
				case 1:	//���Դ
					lightColor += CalculatePointLight(u_Lights[i], cameraDir);	//������Դ����
					break;
				case 2:	//�۹�Դ
					lightColor += CalculateSpotLight(u_Lights[i], cameraDir);	//�����۹�Դ����
					break;
			}
		}
		objectColor = vec4(u_Material.Color.rgb * lightColor, 1.0f);	//������ɫ��ӹ�����ɫ
	}

	color = objectColor;	//��ɫ������0�����ɫ

	objectID = v_ObjectID;
}