#version 330 core
			
layout(location = 0) out vec4 color;	//ƬԪ�����ɫ

in vec3 v_FragPos;	//ƬԪλ��
in vec4 v_Color;	//��ɫ
in vec3 v_Normal;	//����

uniform vec3 u_AmbientColor;	//��������ɫ 
uniform vec3 u_LightColor;		//�ƹ���ɫ
uniform vec3 u_LightDir;		//�ƹ����䷽��
uniform vec3 u_CameraPos;		//���λ��
			
void main()
{
	vec3 cameraDir = normalize(u_CameraPos - v_FragPos);		//ָ���������

	//��������ɫ
	float diffuseIntensity = max(dot(-u_LightDir, v_Normal), 0);	//������ǿ�� [0, 1]
	vec3 diffuseColor =  diffuseIntensity * u_LightColor;			//��������ɫ = ������ǿ�� * �ƹ���ɫ

	//���淴����ɫ
	vec3 reflectDir = reflect(u_LightDir, v_Normal);							//����ⷽ��
	float specularIntensity = pow(max(dot(reflectDir, cameraDir), 0), 32.0f);	//���淴��ǿ�� [0, 1]
	vec3 specularColor = specularIntensity * u_LightColor;						//���淴����ɫ =  ���淴��ǿ�� * �ƹ���ɫ

	vec3 ambientColor = u_AmbientColor * v_Color.rgb;

	color = v_Color * vec4(u_AmbientColor + diffuseColor + specularColor, 1.0f);
}