#version 330 core
			
layout(location = 0) out vec4 color;	//ƬԪ�����ɫ

in vec3 v_FragPos;	//ƬԪλ��
in vec4 v_Color;	//��ɫ
in vec3 v_Normal;	//����

uniform vec3 u_AmbientColor;	//��������ɫ 
uniform vec3 u_LightColor;		//�ƹ���ɫ
uniform vec3 u_LightPos;		//�ƹ�λ��
uniform vec3 u_CameraPos;		//���λ��
			
void main()
{
	vec3 cameraDir = normalize(u_CameraPos - v_FragPos);	//ָ���������
	vec3 lightDir = normalize(u_LightPos - v_FragPos);		//ָ��ƹⷽ��

	//��������ɫ
	float diffuseIntensity = max(dot(lightDir, v_Normal), 0);	//������ǿ�� [0, 1]
	vec3 diffuseColor =  diffuseIntensity * u_LightColor;		//��������ɫ = ������ǿ�� * �ƹ���ɫ

	//���淴����ɫ
	vec3 reflectDir = reflect(-lightDir, v_Normal);								//����ⷽ��
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), 32.0f);	//���淴��ǿ�� [0, 1]		�����32
	vec3 specularColor = specularIntensity * u_LightColor;						//���淴����ɫ =  ���淴��ǿ�� * �ƹ���ɫ

	vec3 ambientColor = u_AmbientColor * v_Color.rgb;

	vec3 resultColor = v_Color.rgb * (u_AmbientColor + diffuseColor + specularColor);

	color = vec4(resultColor, 1.0f);
}