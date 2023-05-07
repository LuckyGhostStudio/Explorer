#version 450 core
			
layout(location = 0) out vec4 f_Color;	    //��ɫ������0ƬԪ�����ɫ
layout(location = 1) out int f_ObjectID;	//��ɫ������1ƬԪ���ֵ������id

in vec2 v_ScreenCoord;  //��Ļ���� [0, 1]

//��������
float PHI = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
float PI  = 3.14159265358979323846264 * 00000.1; // PI
float SQ2 = 1.41421356237309504880169 * 10000.0; // Square Root of Two

float gold_noise(in vec2 coordinate, in float seed)
{
    return fract(tan(distance(coordinate * (seed + PHI), vec2(PHI, PI))) * SQ2);
}

//�������vec3ƫ��
vec3 GetRandomDirectionOffset(vec3 direction)
{
	vec3 random = normalize(vec3(
                0.5 - gold_noise(v_ScreenCoord, direction.x * 10000.0),
                0.5 - gold_noise(v_ScreenCoord, direction.y * 10000.0),
                0.5 - gold_noise(v_ScreenCoord, direction.z * 10000.0)));

	return random;
}

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

//���ʣ����������
struct PBRMaterial
{
    vec3 Albedo;        //������
    float Roughness;    //�ֲڶ�
};

//����
struct Sphere
{
    vec3 Center;
    float Radius;

    int MaterialIndex;
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
    PBRMaterial Materials[100];
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

//������У���������ʱ���� ���ػ����������ʱ��ƬԪ������Ϣ
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
        //����
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
    int bounces = 5;    //���߷������
    //�������߷������
    for(int i = 0; i < bounces; i++){
        HitPayload payload = TraceRay(ray); //׷�ٹ��ߣ�������߻�����������������Ϣ

        //δ�������� ���������ɫ ֹͣ׷��
        if(payload.HitDistance < 0.0){
            vec3 skyColor = vec3(0.6, 0.7, 0.9);
            color += skyColor * multiplier;
            break;
        }

        //�������
        vec3 lightDir = normalize(vec3(-1, -1, -1));	                        //���շ���ƽ�й⣩ TODO ����Ӳ�ͬ��Դ
	    float lightIntensity = max(dot(payload.WorldNormal, -lightDir), 0.0f);	//����ǿ��[0, 1]
    
        const Sphere sphere = u_Scene.Spheres[payload.ObjectIndex];             //���߻��е�����
        const PBRMaterial material = u_Scene.Materials[sphere.MaterialIndex];   //������Ĳ���

	    vec3 sphereColor = material.Albedo; //�������ɫ
	    sphereColor *= lightIntensity;      //������ɫ
        
        color += sphereColor * multiplier;  //�ۼ���ɫ
        multiplier *= 0.5f;                 //�����ǿ�ȱ�������

        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001;  //���·������Դ�� ��ǰ����ƬԪλ�ã��ط��߷���ƫ�ƣ�ȷ�������е�ǰ���壩

        //���·�����߷��� ��ǰ���߷��� �ڵ�ǰ����ƬԪλ�÷��䣨�ۼ� �ֲڶ� * �������ƫ��vec3��
        ray.Direction = reflect(ray.Direction, normalize(payload.WorldNormal + material.Roughness * GetRandomDirectionOffset(ray.Direction)));
    }

    return vec4(color, 1.0);
}

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    f_Color = PerFrag(coord);   //���ݻ��������ƬԪ������Ϣ���� ƬԪ�Ĺ��ռ���
}