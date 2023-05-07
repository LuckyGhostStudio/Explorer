#version 450 core
			
layout(location = 0) out vec4 f_Color;	    //颜色缓冲区0片元输出颜色
layout(location = 1) out int f_ObjectID;	//颜色缓冲区1片元输出值：物体id

in vec2 v_ScreenCoord;  //屏幕坐标 [0, 1]

//噪声函数
float PHI = 1.61803398874989484820459 * 00000.1; // Golden Ratio   
float PI  = 3.14159265358979323846264 * 00000.1; // PI
float SQ2 = 1.41421356237309504880169 * 10000.0; // Square Root of Two

float gold_noise(in vec2 coordinate, in float seed)
{
    return fract(tan(distance(coordinate * (seed + PHI), vec2(PHI, PI))) * SQ2);
}

//计算随机vec3偏移
vec3 GetRandomDirectionOffset(vec3 direction)
{
	vec3 random = normalize(vec3(
                0.5 - gold_noise(v_ScreenCoord, direction.x * 10000.0),
                0.5 - gold_noise(v_ScreenCoord, direction.y * 10000.0),
                0.5 - gold_noise(v_ScreenCoord, direction.z * 10000.0)));

	return random;
}

//光线
struct Ray
{
    vec3 Origin;
    vec3 Direction;
};

vec3 GetRayHitPoint(Ray ray, float t)
{
    return ray.Origin + ray.Direction * t;
}

//材质：基于物理的
struct PBRMaterial
{
    vec3 Albedo;        //反照率
    float Roughness;    //粗糙度
};

//球体
struct Sphere
{
    vec3 Center;
    float Radius;

    int MaterialIndex;
};

//相机
struct Camera
{
    vec3 Position;      //位置
    vec2 ViewportSize;  //视口大小

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
};

//源点与光线和球最近交点距离
float GetSphereHitDistance(Sphere sphere, Ray ray)
{
    vec3 origin = ray.Origin - sphere.Center;   //源点偏移球体位置
	
	float a = dot(ray.Direction, ray.Direction);					//二次项系数：direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
	float b = 2.0 * dot(origin, ray.Direction);						//一次项系数：2 * (origin.x * direction.x + origin.y * direction.y + origin.z * direction.z)
	float c = dot(origin, origin) - sphere.Radius * sphere.Radius;	//常数项：origin.x ^ 2 + origin.y ^ 2 + origin.z ^ 2 - radius ^ 2

	float discriminant = b * b - 4.0f * a * c;		//射线与球交点判别式

    if(discriminant < 0) return -1; //无交点

    float t0 = (-b + sqrt(discriminant)) / 2.0f * a;		//源点与交点0距离  TODO
	float closetT = (-b - sqrt(discriminant)) / 2.0f * a;	//源点与最近交点的距离

	return closetT; //最近交点距离
}

struct Scene
{
    int ObjectCount;
    Sphere Spheres[100];
    PBRMaterial Materials[100];
};

//击中片元信息
struct HitPayload
{
    float HitDistance;  //击中距离
    vec3 WorldPosition; //位置
    vec3 WorldNormal;   //法线

    int ObjectIndex;    //击中物体索引
};

uniform Camera u_Camera;
uniform Scene u_Scene;

//返回射线方向：屏幕坐标 [-1, 1]
vec3 GetRayDirection(vec2 coord)
{
	//屏幕空间->世界空间
	vec4 target = inverse(u_Camera.ProjectionMatrix) * vec4(coord.x, coord.y, 1, 1);
	vec3 rayDirection = vec3(inverse(u_Camera.ViewMatrix) * vec4(normalize(vec3(target) / target.w), 0)); //世界空间

    return rayDirection;
}

//最近命中：击中物体时调用 返回击中最近物体时的片元负载信息
HitPayload ClosetHit(Ray ray, float hitDistance, int objectIndex)
{
    HitPayload payload; //击中片元负载信息
    payload.HitDistance = hitDistance;
    payload.ObjectIndex = objectIndex;

    const Sphere closetSphere = u_Scene.Spheres[objectIndex]; //最近击中球体
    
    payload.WorldPosition = GetRayHitPoint(ray, hitDistance) - closetSphere.Center;	//最近交点坐标：偏移球心位置（变换到0,0点）
	payload.WorldNormal = normalize(payload.WorldPosition);		//交点法线
    
    payload.WorldPosition += closetSphere.Center;   //恢复到相对于球心位置

	return payload;
}

//未击中物体时调用
HitPayload Miss(Ray ray)
{
    HitPayload payload; //未击中片元负载信息
    payload.HitDistance = -1;   //击中距离为 -1
    return payload;
}

//追踪光线：计算光线击中物体与否产生的信息
HitPayload TraceRay(Ray ray)
{
    int closetSphereIndex = -1;     //击中的最近球体索引
    float hitDistance = 340282346638528860000000000000000000000.0;  //击中距离：最大float

    for(int i = 0; i < u_Scene.ObjectCount; i++){
        Sphere sphere = u_Scene.Spheres[i];

        float closetT = GetSphereHitDistance(sphere, ray);    //源点与最近交点距离

        //当前球没被击中
        if (closetT < 0){
            continue;
        }
        //击中
        if(closetT > 0.0 && closetT < hitDistance){
            hitDistance = closetT;  //更新击中距离
            closetSphereIndex = i;  //更新最近的球索引
        }
    }
    
    //最近球体不存在
    if(closetSphereIndex < 0) return Miss(ray);    //未击中物体

    return ClosetHit(ray, hitDistance, closetSphereIndex);  //击中球体
}

//光线发生器：根据击中物体的片元负载信息进行 片元的光照计算
vec4 PerFrag(vec2 coord)
{
    Ray ray;    //相机指向片元的光线
    ray.Origin = u_Camera.Position;
    ray.Direction = GetRayDirection(coord);   

    vec3 color = vec3(0.0);

    float multiplier = 1.0;
    int bounces = 5;    //光线反射次数
    //迭代光线反射次数
    for(int i = 0; i < bounces; i++){
        HitPayload payload = TraceRay(ray); //追踪光线：计算光线击中物体与否产生的信息

        //未击中物体 计算天空颜色 停止追踪
        if(payload.HitDistance < 0.0){
            vec3 skyColor = vec3(0.6, 0.7, 0.9);
            color += skyColor * multiplier;
            break;
        }

        //计算光照
        vec3 lightDir = normalize(vec3(-1, -1, -1));	                        //光照方向（平行光） TODO 待添加不同光源
	    float lightIntensity = max(dot(payload.WorldNormal, -lightDir), 0.0f);	//光照强度[0, 1]
    
        const Sphere sphere = u_Scene.Spheres[payload.ObjectIndex];             //光线击中的球体
        const PBRMaterial material = u_Scene.Materials[sphere.MaterialIndex];   //该球体的材质

	    vec3 sphereColor = material.Albedo; //球表面颜色
	    sphereColor *= lightIntensity;      //球体颜色
        
        color += sphereColor * multiplier;  //累加颜色
        multiplier *= 0.5f;                 //反射光强度倍数削减

        ray.Origin = payload.WorldPosition + payload.WorldNormal * 0.0001;  //更新反射光线源点 当前击中片元位置：沿法线方向偏移（确保不击中当前物体）

        //更新反射光线方向 当前光线方向 在当前击中片元位置反射（累加 粗糙度 * 随机方向偏移vec3）
        ray.Direction = reflect(ray.Direction, normalize(payload.WorldNormal + material.Roughness * GetRandomDirectionOffset(ray.Direction)));
    }

    return vec4(color, 1.0);
}

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    f_Color = PerFrag(coord);   //根据击中物体的片元负载信息进行 片元的光照计算
}