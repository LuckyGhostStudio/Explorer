#include "exppch.h"
#include "SceneSerializer.h"

#include "Object.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"

#include <fstream>
#include <yaml-cpp/yaml.h>
//TODO:添加Mesh序列化和反序列化
//TODO:添加Material序列化和反序列化
namespace YAML 
{
	/// <summary>
	/// vec2转换
	/// </summary>
	template<>
	struct convert<glm::vec2>
	{
		/// <summary>
		/// 将vec2转换为YAML的节点
		/// </summary>
		/// <param name="rhs">vec2类型</param>
		/// <returns>结点</returns>
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		/// <summary>
		/// 将YAML结点类型转换为vec2
		/// </summary>
		/// <param name="node">结点</param>
		/// <param name="rhs">vec2</param>
		/// <returns>是否转换成功</returns>
		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	/// <summary>
	/// vec3转换
	/// </summary>
	template<>
	struct convert<glm::vec3>
	{
		/// <summary>
		/// 将vec3转换为YAML的节点
		/// </summary>
		/// <param name="rhs">vec3类型</param>
		/// <returns>结点</returns>
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		/// <summary>
		/// 将YAML结点类型转换为vec3
		/// </summary>
		/// <param name="node">结点</param>
		/// <param name="rhs">vec3</param>
		/// <returns>是否转换成功</returns>
		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	/// <summary>
	/// vec4转换
	/// </summary>
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Explorer
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;	//流 [x,y]
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;	//流 [x,y,z]
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;	//流 [x,y,z,w]
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene> scene) :m_Scene(scene)
	{

	}

	/// <summary>
	/// 序列化物体
	/// </summary>
	/// <param name="out">发射器</param>
	/// <param name="object">物体</param>
	static void SerializeObject(YAML::Emitter& out, Object object)
	{
		out << YAML::BeginMap;	//开始物体Map
		out << YAML::Key << "Object" << YAML::Value << "12837192831273"; // TODO: Object ID goes here

		//Name组件
		if (object.HasComponent<Name>()){
			out << YAML::Key << "Name Component";
			out << YAML::BeginMap;	//开始Name组件Map
			
			auto& name = object.GetComponent<Name>().m_Name;	//Name组件数据
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap;	//结束Name组件Map
		}

		//Transform组件
		if (object.HasComponent<Transform>()) {
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;	//开始Transform组件Map

			//Transform组件数据
			auto& transform = object.GetComponent<Transform>();

			out << YAML::Key << "Position" << YAML::Value << transform.m_Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.m_Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.m_Scale;

			out << YAML::EndMap;	//结束Transform组件Map
		}

		//Camera组件
		if (object.HasComponent<Camera>())
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap; //开始Camera组件Map

			auto& camera = object.GetComponent<Camera>();

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::Key << "Primary" << YAML::Value << camera.m_Primary;

			out << YAML::EndMap; //结束Camera组件Map
		}

		//Light组件
		if (object.HasComponent<Light>())
		{
			out << YAML::Key << "Light Component";
			out << YAML::BeginMap; //开始Light组件Map

			auto& light = object.GetComponent<Light>();

			out << YAML::Key << "Type" << YAML::Value << (int)light.GetType();
			out << YAML::Key << "Range" << YAML::Value << light.GetRange();
			out << YAML::Key << "SpotOuterAngle" << YAML::Value << light.GetSpotOuterAngle();
			out << YAML::Key << "SpotInnerAngle" << YAML::Value << light.GetSpotInnerAngle();
			out << YAML::Key << "Color" << YAML::Value << light.m_Color;
			out << YAML::Key << "Intensity" << YAML::Value << light.GetIntensity();
			out << YAML::Key << "RenderShadow" << YAML::Value << light.m_RenderShadow;

			out << YAML::EndMap; //结束Light组件Map
		}
		
		//Mesh组件
		if (object.HasComponent<Mesh>())
		{
			out << YAML::Key << "Mesh Component";
			out << YAML::BeginMap;	//开始Mesh组件Map

			auto& mesh = object.GetComponent<Mesh>();
			out << YAML::Key << "Type" << YAML::Value << (int)mesh.GetType();
			//TODO:添加其他属性

			out << YAML::EndMap;	//结束Mesh组件Map
		}

		//Material组件
		if (object.HasComponent<Material>())
		{
			out << YAML::Key << "Material Component";
			out << YAML::BeginMap;	//开始Material组件Map

			auto& material = object.GetComponent<Material>();
			
			//TODO:添加Shader类型
			out << YAML::Key << "Color" << YAML::Value << material.m_Color;									//颜色
			out << YAML::Key << "AlbedoTextureExist" << YAML::Value << material.m_AlbedoTextureExist;		//Albedo是否存在
			out << YAML::Key << "SpecularTextureExist" << YAML::Value << material.m_SpecularTextureExist;	//Specular是否存在								//颜色
			if (material.m_AlbedoTextureExist) {
				out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material.GetAlbedoTexture()->GetPath();		//Albedo贴图路径
			}
			if (material.m_SpecularTextureExist) {
				out << YAML::Key << "SpecularTexturePath" << YAML::Value << material.GetSpecularTexture()->GetPath();	//Specular贴图路径
			}
			out << YAML::Key << "Shininess" << YAML::Value << material.GetShininess();						//反光度
			out << YAML::Key << "Tiling" << YAML::Value << material.m_Tiling;								//纹理平铺因子
			out << YAML::Key << "Offset" << YAML::Value << material.m_Offset;								//纹理平铺因子

			out << YAML::EndMap;	//结束Material组件Map
		}

		//SpriteRenderer组件
		if (object.HasComponent<SpriteRenderer>())
		{
			out << YAML::Key << "SpriteRenderer Component";
			out << YAML::BeginMap;	//开始SpriteRenderer组件Map

			auto& spriteRenderer = object.GetComponent<SpriteRenderer>();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.m_Color;

			out << YAML::EndMap;	//结束SpriteRenderer组件Map
		}

		out << YAML::EndMap;	//结束物体Map
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;		//发射器
		out << YAML::BeginMap;	//开始场景Map
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";			//场景：场景名
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;		//物体：开始物体序列
		m_Scene->m_Registry.each([&](auto objectID)		//遍历场景注册表所有物体
		{
			Object object = { objectID, m_Scene.get() };
			if (!object)
				return;

			SerializeObject(out, object);			//序列化物体
		});
		out << YAML::EndSeq;	//结束物体序列
		out << YAML::EndMap;	//结束场景Map

		std::ofstream fout(filepath);	//输出流
		fout << out.c_str();			//输出序列化结果到输出流文件
	}
	
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//未实现
	}
	
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);	//文件输入流
		std::stringstream strStream;
		strStream << stream.rdbuf();	//文件读入字符串

		YAML::Node data = YAML::Load(strStream.str());	//加载到YMAL结点
		//Scene节点不存在
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();	//场景名
		EXP_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		YAML::Node objects = data["Objects"];	//物体序列结点

		if (objects) {	//物体结点存在
			for (auto object : objects) {	//遍历结点下所有物体
				uint64_t uuid = object["Object"].as<uint64_t>(); //物体uid TODO

				std::string name;
				auto nameNode = object["Name Component"];	//Name组件结点
				if (nameNode) {
					name = nameNode["Name"].as<std::string>();	//物体名
				}

				EXP_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Object deserializedObject = m_Scene->CreateEmptyObject(name);	//创建的空物体

				//Transform组件结点
				auto transformNode = object["Transform Component"];
				if (transformNode){
					auto& transform = deserializedObject.GetComponent<Transform>();	//获取Transform组件
					//设置变换组件数据
					transform.m_Position = transformNode["Position"].as<glm::vec3>();
					transform.m_Rotation = transformNode["Rotation"].as<glm::vec3>();
					transform.m_Scale = transformNode["Scale"].as<glm::vec3>();
				}

				//Camera组件结点
				auto cameraNode = object["Camera Component"];
				if (cameraNode)
				{
					auto& camera = deserializedObject.AddComponent<Camera>();	//添加Camera组件

					//设置相机组件数据
					camera.SetProjectionType((Camera::ProjectionType)cameraNode["ProjectionType"].as<int>());
					camera.SetSize(cameraNode["Size"].as<float>());
					camera.SetFOV(cameraNode["FOV"].as<float>());
					camera.SetNearClip(cameraNode["Near"].as<float>());
					camera.SetFarClip(cameraNode["Far"].as<float>());
					camera.m_Primary = cameraNode["Primary"].as<bool>();
				}

				//Light组件结点
				auto lightNode = object["Light Component"];
				if (lightNode)
				{
					auto& light = deserializedObject.AddComponent<Light>();	//添加Light组件

					//设置光源组件数据
					light.SetType((Light::Type)lightNode["Type"].as<int>());
					light.SetRange(lightNode["Range"].as<float>());
					light.SetSpotOuterAngle(lightNode["SpotOuterAngle"].as<float>());
					light.SetSpotInnerAngle(lightNode["SpotInnerAngle"].as<float>());
					light.m_Color = lightNode["Color"].as<glm::vec3>();
					light.SetIntensity(lightNode["Intensity"].as<float>());
					light.m_RenderShadow = lightNode["RenderShadow"].as<bool>();
				}

				//Mesh组件结点
				auto meshNode = object["Mesh Component"];
				if (meshNode)
				{
					auto& mesh = deserializedObject.AddComponent<Mesh>();	//添加Mesh组件

					//设置网格组件数据
					mesh.SetType((Mesh::Type)meshNode["Type"].as<int>());

					std::vector<Vertex> vertices;	//Mesh顶点
					std::vector<uint32_t> indices;	//Mesh顶点索引

					switch (mesh.GetType())
					{
						case Mesh::Type::None:
							break;
						case Mesh::Type::Other:
							break;
						case Mesh::Type::Cube:
							//正方体顶点
							vertices =
							{
								// ---------位置---------	----------颜色----------		---------法线--------	---纹理坐标---  ID objID
								{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 0 x+
								{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 1 y-
								{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 2 z+

								{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 3
								{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 4
								{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 5

								{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 6
								{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 7
								{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 8

								{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 9
								{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 10
								{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f }, 3, 0 },	// D 11

								{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 12
								{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 13
								{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 0.0f, 0.0f }, 4, 0 },	// E 14

								{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 15
								{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 16
								{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 1.0f }, 5, 0 },	// F 17

								{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 18
								{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 19
								{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 20

								{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 21
								{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 22
								{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 23
							};

							//顶点索引
							indices =
							{
								0, 3, 6,	// A B C x+
								6, 9, 0,	// C D A x+
								18, 15, 12,	// G F E x-
								18, 21, 12,	// G H E x-
								22, 7, 19,	// H C G y+
								7, 10, 22,	// C D H y+
								13, 16, 4,	// E F B y-
								4, 1, 13,	// B A E y-
								23, 14, 2,	// H E A z+
								2, 11, 23,	// A D H z+
								20, 5, 17,	// G B F z-
								5, 20, 8,	// B G C z-
							};

							mesh.AddSubMesh(SubMesh(vertices, indices));	//添加Cube子网格
							break;
						case Mesh::Type::Sphere:
							break;
						case Mesh::Type::Capsule:
							break;
						case Mesh::Type::Cylinder:
							break;
						case Mesh::Type::Plane:
							break;
					}
					//TODO:补充属性
				}

				//Material组件结点
				auto materialNode = object["Material Component"];
				if (materialNode)
				{
					auto& material = deserializedObject.AddComponent<Material>();	//添加Material组件

					//设置光源组件数据
					//TODO:添加Shader类型
					material.m_Color = materialNode["Color"].as<glm::vec4>();
					material.m_AlbedoTextureExist = materialNode["AlbedoTextureExist"].as<bool>();
					material.m_SpecularTextureExist = materialNode["SpecularTextureExist"].as<bool>();
					if (material.m_AlbedoTextureExist) {	//Albedo存在
						material.SetAlbedoTexture(materialNode["AlbedoTexturePath"].as<std::string>());	//加载Albedo贴图
					}
					if (material.m_SpecularTextureExist) {	//Specular存在
						material.SetSpecularTexture(materialNode["SpecularTexturePath"].as<std::string>());	//加载Specular贴图
					}
					material.SetShininess(materialNode["Shininess"].as<float>());
					material.m_Tiling = materialNode["Tiling"].as<glm::vec2>();
					material.m_Offset = materialNode["Offset"].as<glm::vec2>();
				}

				//SpriteRenderer组件结点
				auto spriteRendererNode = object["SpriteRenderer Component"];
				if (spriteRendererNode)
				{
					auto& spriteRenderer = deserializedObject.AddComponent<SpriteRenderer>();	//添加SpriteRenderer组件
					spriteRenderer.m_Color = spriteRendererNode["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}
	
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//未实现
		return false;
	}
}