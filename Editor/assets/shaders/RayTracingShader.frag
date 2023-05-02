#version 450 core
			
layout(location = 0) out vec4 f_Color;	    //��ɫ������0ƬԪ�����ɫ
//layout(location = 1) out int f_ObjectID;	//��ɫ������1ƬԪ���ֵ������id

//����
struct Ray
{
    vec3 Origin;
    vec3 Direction;
};

vec3 GetRayHitPoint(Ray ray, float t)
{
    return ray.Origin + ray.Direction * t;
}

//����
struct Sphere
{
    vec3 Center;
    float Radius;

    vec3 Albedo;
};

//���
struct Camera
{
    vec3 Position;      //λ��
    vec2 ViewportSize;  //�ӿڴ�С

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
};

//Դ������ߺ�������������
float GetSphereHitDistance(Sphere sphere, Ray ray)
{
    vec3 origin = ray.Origin - sphere.Center;   //Դ��ƫ������λ��
	
	float a = dot(ray.Direction, ray.Direction);					//������ϵ����direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
	float b = 2.0 * dot(origin, ray.Direction);						//һ����ϵ����2 * (origin.x * direction.x + origin.y * direction.y + origin.z * direction.z)
	float c = dot(origin, origin) - sphere.Radius * sphere.Radius;	//�����origin.x ^ 2 + origin.y ^ 2 + origin.z ^ 2 - radius ^ 2

	float discriminant = b * b - 4.0f * a * c;		//�������򽻵��б�ʽ

    if(discriminant < 0) return -1; //�޽���

    float t0 = (-b + sqrt(discriminant)) / 2.0f * a;		//Դ���뽻��0����  TODO
	float closetT = (-b - sqrt(discriminant)) / 2.0f * a;	//Դ�����������ľ���

	return closetT; //����������
}

struct Scene
{
    int ObjectCount;
    Sphere Spheres[100];
};

//����ƬԪ��Ϣ
struct HitPayload
{
    float HitDistance;  //���о���
    vec3 WorldPosition; //λ��
    vec3 WorldNormal;   //����

    int ObjectIndex;    //������������
};

uniform Camera u_Camera;
uniform Scene u_Scene;

//�������߷�����Ļ���� [-1, 1]
vec3 GetRayDirection(vec2 coord)
{
	//��Ļ�ռ�->����ռ�
	vec4 target = inverse(u_Camera.ProjectionMatrix) * vec4(coord.x, coord.y, 1, 1);
	vec3 rayDirection = vec3(inverse(u_Camera.ViewMatrix) * vec4(normalize(vec3(target) / target.w), 0)); //����ռ�

    return rayDirection;
}

//���������ɫ������������ʱ���� ���ػ����������ʱ��ƬԪ������Ϣ
HitPayload ClosetHit(Ray ray, float hitDistance, int objectIndex)
{
    HitPayload payload; //����ƬԪ������Ϣ
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere closetSphere = u_Scene.Spheres[objectIndex]; //�����������
    
    payload.WorldPosition = GetRayHitPoint(ray, hitDistance) - closetSphere.Center;	//����������꣺ƫ������λ�ã��任��0,0�㣩
	payload.WorldNormal = normalize(payload.WorldPosition);		//���㷨��
    
    payload.WorldPosition += closetSphere.Center;   //�ָ������������λ��

	return payload;
}

//δ��������ʱ����
HitPayload Miss(Ray ray)
{
    HitPayload payload; //δ����ƬԪ������Ϣ
    payload.HitDistance = -1;   //���о���Ϊ -1
    return payload;
}

//׷�ٹ��ߣ�������߻�����������������Ϣ
HitPayload TraceRay(Ray ray)
{
    int closetSphereIndex = -1;     //���е������������
    float hitDistance = 340282346638528860000000000000000000000.0;  //���о��룺���float

    for(int i = 0; i < u_Scene.ObjectCount; i++){
        Sphere sphere = u_Scene.Spheres[i];

        float closetT = GetSphereHitDistance(sphere, ray);    //Դ��������������

        //��ǰ��û������
        if (closetT < 0){
            continue;
        }

        if(closetT > 0.0 && closetT < hitDistance){
            hitDistance = closetT;  //���»��о���
            closetSphereIndex = i;  //���������������
        }
    }
    
    //������岻����
    if(closetSphereIndex < 0) return Miss(ray);    //δ��������

    return ClosetHit(ray, hitDistance, closetSphereIndex);  //��������
}

//���߷����������ݻ��������ƬԪ������Ϣ���� ƬԪ�Ĺ��ռ���
vec4 PerFrag(vec2 coord)
{
    Ray ray;    //���ָ��ƬԪ�Ĺ���
    ray.Origin = u_Camera.Position;
    ray.Direction = GetRayDirection(coord);   

    vec3 color = vec3(0.0);

    float multiplier = 1.0;
    int bounces = 2;    //���߷������
    //�������߷������
    for(int i = 0; i < bounces; i++){
        HitPayload payload = TraceRay(ray); //׷�ٹ��ߣ�������߻�����������������Ϣ

        //δ�������� ���������ɫ ֹͣ׷��
        if(payload.HitDistance < 0.0){
            vec3 skyColor = vec3(0.1, 0.1, 0.1);
            color += skyColor * multiplier;
            break;
        }

        //�������
        vec3 lightDir = normalize(vec3(-1, -1, -1));	                        //���շ���
	    float lightIntensity = max(dot(payload.WorldNormal, -lightDir), 0.0f);	//����ǿ��[0, 1]
    
        Sphere sphere = u_Scene.Spheres[payload.ObjectIndex];   //���߻��е�����

	    vec3 sphereColor = sphere.Albedo;
	    sphereColor *= lightIntensity;  //������ɫ

        color += sphereColor * multiplier;  //�ۼ���ɫ
        multiplier *= 0.7f;                 //�����ǿ�ȱ�������

        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001;  //���·������Դ�� ��ǰ����ƬԪλ�ã��ط��߷���ƫ�ƣ�ȷ�������е�ǰ���壩
        ray.Direction = reflect(ray.Direction, payload.WorldNormal);        //���·�����߷��� ��ǰ���߷��� �ڵ�ǰ����ƬԪλ�÷���
    }

    return vec4(color, 1.0);
}

in vec2 v_ScreenCoord;  //��Ļ���� [0, 1]

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    f_Color = PerFrag(coord);   //���ݻ��������ƬԪ������Ϣ���� ƬԪ�Ĺ��ռ���
}