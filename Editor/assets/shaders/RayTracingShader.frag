#version 450 core
			
layout(location = 0) out vec4 f_Color;	    //颜色缓冲区0片元输出颜色
//layout(location = 1) out int f_ObjectID;	//颜色缓冲区1片元输出值：物体id

//光线
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

//球体
struct Sphere
{
    vec3 Center;
    float Radius;

    vec3 Albedo;
};

Sphere CreateSphere(vec3 center, float radius, vec3 albedo)
{
    Sphere sphere;
    sphere.Center = center;
    sphere.Radius = radius;
    sphere.Albedo = albedo;
    return sphere;
}

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
};

//追踪光线：计算光线与球面交点颜色
vec4 TraceRay(Scene scene, Ray ray)
{
    if(scene.ObjectCount == 0) return vec4(0.15, 0.15, 0.15, 1);  //背景色 

    Sphere closetSphere;            //最近的球体
    bool closetSphereEmpty = true;  //最近的球体为空
    float hitDistance = 340282346638528860000000000000000000000.0;  //击中距离：最大float

    for(int i = 0; i < scene.ObjectCount; i++){
        Sphere sphere = scene.Spheres[i];

        float closetT = GetSphereHitDistance(sphere, ray);    //源点与最近交点距离

        //当前球没被击中
        if (closetT < 0){
            continue;
        }

        if(closetT < hitDistance){
            hitDistance = closetT;  //更新击中距离
            closetSphere = sphere;  //更新最近的球
            closetSphereEmpty = false;
        }
    }
    
    //最近球体不存在
    if(closetSphereEmpty) return vec4(0.15, 0.15, 0.15, 1);  //背景色

	vec3 hitPoint = GetRayHitPoint(ray, hitDistance) - closetSphere.Center;	//最近交点坐标：偏移球心位置
	vec3 normal = normalize(hitPoint);		//交点法线
    
	vec3 lightDir = normalize(vec3(-1, -1, -1));	//光照方向
	float lightIntensity = max(dot(normal, -lightDir), 0.0f);	//光照强度[0, 1]
    
	vec3 sphereColor = closetSphere.Albedo;
	sphereColor *= lightIntensity;
	return vec4(sphereColor, 1);
}

//相机
struct Camera
{
    vec3 Position;      //位置
    vec2 ViewportSize;  //视口大小

    mat4 ProjectionMatrix;
    mat4 ViewMatrix;
};

//返回射线方向：相机, 屏幕坐标 [-1, 1]
vec3 GetRayDirection(Camera camera, vec2 coord)
{
	//屏幕空间->世界空间
	vec4 target = inverse(camera.ProjectionMatrix) * vec4(coord.x, coord.y, 1, 1);
	vec3 rayDirection = vec3(inverse(camera.ViewMatrix) * vec4(normalize(vec3(target) / target.w), 0)); //世界空间

    return rayDirection;
}

uniform Camera u_Camera;
uniform Scene u_Scene;

in vec2 v_ScreenCoord;  //屏幕坐标 [0, 1]

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    Ray ray = CreateRay(u_Camera.Position, GetRayDirection(u_Camera, coord));   //相机指向片元的光线

    f_Color = vec4(TraceRay(u_Scene, ray));  //计算光线与球交点颜色
}