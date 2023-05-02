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
};

Sphere CreateSphere(vec3 center, float radius)
{
    Sphere sphere;
    sphere.Center = center;
    sphere.Radius = radius;
    return sphere;
}

//源点与最近交点距离
float GetSphereHitDistance(Sphere sphere, Ray ray)
{
    vec3 originToCenter = ray.Origin - sphere.Center;
	
	float a = dot(ray.Direction, ray.Direction);									//二次项系数：direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
	float b = 2.0 * dot(originToCenter, ray.Direction);								//一次项系数：2 * (originToCenter.x * direction.x + originToCenter.y * direction.y + originToCenter.z * direction.z)
	float c = dot(originToCenter, originToCenter) - sphere.Radius * sphere.Radius;	//常数项：originToCenter.x ^ 2 + originToCenter.y ^ 2 + originToCenter.z ^ 2 - radius ^ 2

	float discriminant = b * b - 4.0f * a * c;		//射线与球交点判别式

    if(discriminant < 0) return -1; //无交点

    float t0 = (-b + sqrt(discriminant)) / 2.0f * a;		//源点与交点0距离  TODO
	float closetT = (-b - sqrt(discriminant)) / 2.0f * a;	//源点与最近交点的距离

	return closetT; //最近交点距离
}

//追踪光线：计算光线与球面交点颜色
vec4 TraceRay(Ray ray)
{
    Sphere sphere = CreateSphere(vec3(0, 0, 0), 0.5);

    float t = GetSphereHitDistance(sphere, ray);    //源点与最近交点距离

    //球没被击中
    if (t < 0){
        return vec4(0.15, 0.15, 0.15, 1);  //背景色
    }
    
	vec3 hitPoint = GetRayHitPoint(ray, t);	//最近交点坐标
	vec3 normal = normalize(hitPoint);		//交点法线
    
	vec3 lightDir = normalize(vec3(-1, -1, -1));	//光照方向
	float d = max(dot(normal, -lightDir), 0.0f);	//光照强度[0, 1]
    
	vec3 sphereColor = vec3(1, 0, 1);
	sphereColor *= d;
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

in vec2 v_ScreenCoord;  //屏幕坐标 [0, 1]

void main()
{
    vec2 coord = v_ScreenCoord * 2.0 - 1.0; // To [-1, 1]

    Ray ray = CreateRay(u_Camera.Position, GetRayDirection(u_Camera, coord));   //相机指向片元的光线

    f_Color = vec4(TraceRay(ray));  //计算光线与球交点颜色
}