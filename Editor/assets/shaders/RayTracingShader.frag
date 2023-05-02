#version 450 core
			
layout(location = 0) out vec4 f_Color;	    //��ɫ������0ƬԪ�����ɫ
//layout(location = 1) out int f_ObjectID;	//��ɫ������1ƬԪ���ֵ������id

//����
struct Ray
{
    vec3 Origin;
    vec3 Direction;
};

Ray CreateRay(vec3 origin, vec3 direction)
{
    Ray ray;
    ray.Origin = origin;
    ray.Direction = direction;
    return ray;
}

vec3 GetRayHitPoint(Ray ray, float t)
{
    return ray.Origin + ray.Direction * t;
}

//����
struct Sphere
{
    vec3 Center;
    float Radius;
};

Sphere CreateSphere(vec3 center, float radius)
{
    Sphere sphere;
    sphere.Center = center;
    sphere.Radius = radius;
    return sphere;
}

//Դ��������������
float GetSphereHitDistance(Sphere sphere, Ray ray)
{
    vec3 originToCenter = ray.Origin - sphere.Center;
	
	float a = dot(ray.Direction, ray.Direction);									//������ϵ����direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
	float b = 2.0 * dot(originToCenter, ray.Direction);								//һ����ϵ����2 * (originToCenter.x * direction.x + originToCenter.y * direction.y + originToCenter.z * direction.z)
	float c = dot(originToCenter, originToCenter) - sphere.Radius * sphere.Radius;	//�����originToCenter.x ^ 2 + originToCenter.y ^ 2 + originToCenter.z ^ 2 - radius ^ 2

	float discriminant = b * b - 4.0f * a * c;		//�������򽻵��б�ʽ

    if(discriminant < 0) return -1; //�޽���

    float t0 = (-b + sqrt(discriminant)) / 2.0f * a;		//Դ���뽻��0����  TODO
	float closetT = (-b - sqrt(discriminant)) / 2.0f * a;	//Դ�����������ľ���

	return closetT; //����������
}

//׷�ٹ��ߣ�������������潻����ɫ
vec4 TraceRay(Ray ray)
{
    Sphere sphere = CreateSphere(vec3(0, 0, 0), 0.5);

    float t = GetSphereHitDistance(sphere, ray);    //Դ��������������

    //��û������
    if (t < 0){
        return vec4(0.15, 0.15, 0.15, 1);  //����ɫ
    }
    
	vec3 hitPoint = GetRayHitPoint(ray, t);	//�����������
	vec3 normal = normalize(hitPoint);		//���㷨��
    
	vec3 lightDir = normalize(vec3(-1, -1, -1));	//���շ���
	float d = max(dot(normal, -lightDir), 0.0f);	//����ǿ��[0, 1]
    
	vec3 sphereColor = vec3(1, 0, 1);
	sphereColor *= d;
	return vec4(sphereColor, 1);
}

//���
struct Camera
{
    vec3 Position;      //λ��
    vec2 ViewportSize;  //�ӿڴ�С

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
};

//�������߷������, ��Ļ���� [-1, 1]
vec3 GetRayDirection(Camera camera, vec2 coord)
{
	//��Ļ�ռ�->����ռ�
	vec4 target = inverse(camera.ProjectionMatrix) * vec4(coord.x, coord.y, 1, 1);
	vec3 rayDirection = vec3(inverse(camera.ViewMatrix) * vec4(normalize(vec3(target) / target.w), 0)); //����ռ�

    return rayDirection;
}

uniform Camera u_Camera;

in vec2 v_ScreenCoord;  //��Ļ���� [0, 1]

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    Ray ray = CreateRay(u_Camera.Position, GetRayDirection(u_Camera, coord));   //���ָ��ƬԪ�Ĺ���

    f_Color = vec4(TraceRay(ray));  //����������򽻵���ɫ
}