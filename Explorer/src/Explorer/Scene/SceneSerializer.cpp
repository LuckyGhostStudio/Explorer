#include "exppch.h"
#include "SceneSerializer.h"

#include "Object.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/SpriteRenderer.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

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

		//Self组件
		if (object.HasComponent<Self>()){
			out << YAML::Key << "Self Component";
			out << YAML::BeginMap;	//开始Self组件Map
			
			auto& self = object.GetComponent<Self>();	//Self组件数据
			out << YAML::Key << "ObjectName" << YAML::Value << self.GetObjectName();		//物体名
			out << YAML::Key << "ObjectEnable" << YAML::Value << self.GetObjectEnable();	//物体启用状态

			out << YAML::EndMap;	//结束Self组件Map
		}

		//Transform组件
		if (object.HasComponent<Transform>()) {
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;	//开始Transform组件Map

			//Transform组件数据
			auto& transform = object.GetComponent<Transform>();

			out << YAML::Key << "Position" << YAML::Value << transform.GetPosition();
			out << YAML::Key << "Rotation" << YAML::Value << transform.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << transform.GetScale();

			out << YAML::EndMap;	//结束Transform组件Map
		}

		//Camera组件
		if (object.HasComponent<Camera>())
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap; //开始Camera组件Map

			auto& camera = object.GetComponent<Camera>();

			out << YAML::Key << "Enable" << YAML::Value << camera.GetEnable();

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::Key << "Primary" << YAML::Value << camera.IsPrimary();

			out << YAML::EndMap; //结束Camera组件Map
		}

		//Light组件
		if (object.HasComponent<Light>())
		{
			out << YAML::Key << "Light Component";
			out << YAML::BeginMap; //开始Light组件Map

			auto& light = object.GetComponent<Light>();

			out << YAML::Key << "Enable" << YAML::Value << light.GetEnable();

			out << YAML::Key << "Type" << YAML::Value << (int)light.GetType();
			out << YAML::Key << "Range" << YAML::Value << light.GetRange();
			out << YAML::Key << "SpotOuterAngle" << YAML::Value << light.GetSpotOuterAngle();
			out << YAML::Key << "SpotInnerAngle" << YAML::Value << light.GetSpotInnerAngle();
			out << YAML::Key << "Color" << YAML::Value << light.GetColor();
			out << YAML::Key << "Intensity" << YAML::Value << light.GetIntensity();
			out << YAML::Key << "RenderShadow" << YAML::Value << light.GetCastShadow();

			out << YAML::EndMap; //结束Light组件Map
		}
		
		//Mesh组件
		if (object.HasComponent<Mesh>())
		{
			out << YAML::Key << "Mesh Component";
			out << YAML::BeginMap;	//开始Mesh组件Map

			auto& mesh = object.GetComponent<Mesh>();

			out << YAML::Key << "Enable" << YAML::Value << mesh.GetEnable();

			out << YAML::Key << "Type" << YAML::Value << (int)mesh.GetType();

			//不是内置Mesh类型
			if (mesh.GetType() == Mesh::Type::Other) {
				out << YAML::Key << "Name" << YAML::Value << mesh.GetName();

				out << YAML::Key << "SubMeshes" << YAML::Value << YAML::BeginSeq;	//子网格：开始SubMesh序列
			
				//遍历所有子网格
				for (int i = 0; i < mesh.GetSubMeshCount(); i++) {
					SubMesh& subMesh = mesh.GetSubMeshes()[i];

					out << YAML::BeginMap;			//开始SubMesh Map
					out << YAML::Key << "SubMesh" << YAML::Value << i;	//SubMesh结点

					out << YAML::Key << "Vertices" << YAML::Value << YAML::BeginSeq;	//顶点：开始Vertex序列
					
					//顶点数据
					for (int j = 0; j < subMesh.GetVertexCount(); j++) {
						Vertex& vertex = subMesh.GetVertices()[j];

						out << YAML::BeginMap;			//开始Vertex Map
						out << YAML::Key << "Vertex" << YAML::Value << j;	//Vertex结点

						out << YAML::Key << "Position" << YAML::Value << vertex.Position;
						out << YAML::Key << "Color" << YAML::Value << vertex.Color;
						out << YAML::Key << "Normal" << YAML::Value << vertex.Normal;
						out << YAML::Key << "TexCoord" << YAML::Value << vertex.TexCoord;
						out << YAML::Key << "ID" << YAML::Value << vertex.ID;
						out << YAML::Key << "ObjectID" << YAML::Value << vertex.ObjectID;

						out << YAML::EndMap;			//结束Vertex Map
					}

					out << YAML::EndSeq;												//结束Vertex序列

					out << YAML::Key << "VertexIndices" << YAML::Value << YAML::BeginSeq;	//顶点索引：开始VertexIndices序列

					//顶点索引数据
					for (int j = 0; j < subMesh.GetVertexIndexCount(); j++) {
						out << YAML::BeginMap;									//开始VertexIndex Map
						out << YAML::Key << "VertexIndex" << YAML::Value << j;	//VertexIndex结点

						out << YAML::Key << "Index" << YAML::Value << subMesh.GetVertexIndices()[j];

						out << YAML::EndMap;				//结束VertexIndex Map
					}

					out << YAML::EndSeq;												//结束VertexIndices序列

					out << YAML::EndMap;			//结束SubMesh Map
				}
				out << YAML::EndSeq;												//结束SubMesh序列
			}

			out << YAML::EndMap;	//结束Mesh组件Map
		}

		//Material组件
		if (object.HasComponent<Material>())
		{
			out << YAML::Key << "Material Component";
			out << YAML::BeginMap;	//开始Material组件Map

			auto& material = object.GetComponent<Material>();
			
			out << YAML::Key << "Enable" << YAML::Value << material.GetEnable();

			out << YAML::Key << "Shader" << YAML::Value << material.GetShader()->GetName();	//Shader名

			out << YAML::Key << "Color" << YAML::Value << material.GetColor();								//颜色
			out << YAML::Key << "AlbedoTextureExist" << YAML::Value << material.GetAlbedoTextureExist();	//Albedo是否存在
			out << YAML::Key << "SpecularTextureExist" << YAML::Value << material.GetSpecularTextureExist();//Specular是否存在								//颜色
			if (material.GetAlbedoTextureExist()) {
				out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material.GetAlbedoTexture()->GetPath();		//Albedo贴图路径
			}
			if (material.GetSpecularTextureExist()) {
				out << YAML::Key << "SpecularTexturePath" << YAML::Value << material.GetSpecularTexture()->GetPath();	//Specular贴图路径
			}
			out << YAML::Key << "Shininess" << YAML::Value << material.GetShininess();						//反光度
			out << YAML::Key << "Tiling" << YAML::Value << material.GetTiling();							//纹理平铺因子
			out << YAML::Key << "Offset" << YAML::Value << material.GetOffset();							//纹理平铺因子

			out << YAML::EndMap;	//结束Material组件Map
		}

		//SpriteRenderer组件
		if (object.HasComponent<SpriteRenderer>())
		{
			out << YAML::Key << "SpriteRenderer Component";
			out << YAML::BeginMap;	//开始SpriteRenderer组件Map

			auto& spriteRenderer = object.GetComponent<SpriteRenderer>();

			out << YAML::Key << "Enable" << YAML::Value << spriteRenderer.GetEnable();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.GetColor();
			//TODO:待添加Sprite

			out << YAML::EndMap;	//结束SpriteRenderer组件Map
		}

		out << YAML::EndMap;	//结束物体Map
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;		//发射器
		out << YAML::BeginMap;	//开始场景Map
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();	//场景：场景名

		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;		//物体：开始物体序列
		m_Scene->m_Registry.each([&](auto objectID)		//遍历场景注册表所有物体
		{
			Object object = { objectID, m_Scene.get() };
			if (!object)
				return;

			SerializeObject(out, object);			//序列化物体
		});
		out << YAML::EndSeq;	//结束物体序列

		out << YAML::Key << "Environment";	//环境设置结点
		out << YAML::BeginMap;		//开始环境Map

		Environment& environment = m_Scene->GetEnvironment();	//环境对象
		//环境设置数据
		out << YAML::Key << "SkyboxEnable" << YAML::Value << environment.GetSkyboxEnable();		//天空盒启用状态
		out << YAML::Key << "SourceType" << YAML::Value << (int)environment.GetSourceType();	
		out << YAML::Key << "IntensityMultiplier" << YAML::Value << environment.GetIntensityMultiplier();
		out << YAML::Key << "AmbientColor" << YAML::Value << environment.GetAmbientColor();
		
		out << YAML::Key << "Skybox";	//Skybox结点
		out << YAML::BeginMap;			//开始SkyboxMap
		Skybox& skybox = environment.GetSkybox();
		//天空盒参数
		out << YAML::Key << "Shader" << YAML::Value << skybox.GetShader()->GetName();	//着色器名
		out << YAML::Key << "TintColor" << YAML::Value << skybox.GetTintColor();
		out << YAML::Key << "Expose" << YAML::Value << skybox.GetExpose();
		out << YAML::Key << "Rotation" << YAML::Value << skybox.GetRotation();
		//Cubemap 纹理路径
		out << YAML::Key << "CubemapRightPath" << YAML::Value << skybox.GetPreviewTextures()[0]->GetPath();
		out << YAML::Key << "CubemapLeftPath" << YAML::Value << skybox.GetPreviewTextures()[1]->GetPath();
		out << YAML::Key << "CubemapUpPath" << YAML::Value << skybox.GetPreviewTextures()[2]->GetPath();
		out << YAML::Key << "CubemapDownPath" << YAML::Value << skybox.GetPreviewTextures()[3]->GetPath();
		out << YAML::Key << "CubemapFrontPath" << YAML::Value << skybox.GetPreviewTextures()[4]->GetPath();
		out << YAML::Key << "CubemapBackPath" << YAML::Value << skybox.GetPreviewTextures()[5]->GetPath();

		out << YAML::EndMap;			//结束Skybox Map

		out << YAML::EndMap;		//结束环境Map

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
		m_Scene->SetName(sceneName);

		EXP_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		YAML::Node objects = data["Objects"];			//物体序列结点
		YAML::Node environment = data["Environment"];	//环境设置结点

		if (objects) {	//物体结点存在
			for (auto object : objects) {	//遍历结点下所有物体
				uint64_t uuid = object["Object"].as<uint64_t>(); //物体uid TODO

				std::string objectName;
				bool objectEnable;
				auto selfNode = object["Self Component"];	//Self组件结点
				if (selfNode) {
					objectName = selfNode["ObjectName"].as<std::string>();	//物体名
					objectEnable = selfNode["ObjectEnable"].as<bool>();		//物体启用状态
				}

				EXP_CORE_TRACE("Deserialized object with ID = {0}, name = {1}", uuid, objectName);

				Object deserializedObject = m_Scene->CreateEmptyObject(objectName, objectEnable);	//创建的空物体

				//Transform组件结点
				auto transformNode = object["Transform Component"];
				if (transformNode){
					auto& transform = deserializedObject.GetComponent<Transform>();	//获取Transform组件
					
					//设置变换组件数据
					transform.SetPosition(transformNode["Position"].as<glm::vec3>());
					transform.SetRotation(transformNode["Rotation"].as<glm::vec3>());
					transform.SetScale(transformNode["Scale"].as<glm::vec3>());
				}

				//Camera组件结点
				auto cameraNode = object["Camera Component"];
				if (cameraNode)
				{
					auto& camera = deserializedObject.AddComponent<Camera>();	//添加Camera组件

					//设置相机组件数据
					camera.SetEnable(cameraNode["Enable"].as<bool>());	//组件启用状态

					camera.SetProjectionType((Camera::ProjectionType)cameraNode["ProjectionType"].as<int>());
					camera.SetSize(cameraNode["Size"].as<float>());
					camera.SetFOV(cameraNode["FOV"].as<float>());
					camera.SetNearClip(cameraNode["Near"].as<float>());
					camera.SetFarClip(cameraNode["Far"].as<float>());
					camera.SetPrimary(cameraNode["Primary"].as<bool>());
				}

				//Light组件结点
				auto lightNode = object["Light Component"];
				if (lightNode)
				{
					auto& light = deserializedObject.AddComponent<Light>();	//添加Light组件

					//设置光源组件数据
					light.SetEnable(lightNode["Enable"].as<bool>());	//组件启用状态

					light.SetType((Light::Type)lightNode["Type"].as<int>());
					light.SetRange(lightNode["Range"].as<float>());
					light.SetSpotOuterAngle(lightNode["SpotOuterAngle"].as<float>());
					light.SetSpotInnerAngle(lightNode["SpotInnerAngle"].as<float>());
					light.SetColor(lightNode["Color"].as<glm::vec3>());
					light.SetIntensity(lightNode["Intensity"].as<float>());
					light.SetCastShadow(lightNode["RenderShadow"].as<bool>());
				}

				//Mesh组件结点
				auto meshNode = object["Mesh Component"];
				if (meshNode)
				{
					auto& mesh = deserializedObject.AddComponent<Mesh>();	//添加Mesh组件

					//设置网格组件数据
					mesh.SetEnable(meshNode["Enable"].as<bool>());	//组件启用状态

					mesh.SetType((Mesh::Type)meshNode["Type"].as<int>());

					//不是内置Mesh类型
					if (mesh.GetType() == Mesh::Type::Other) {
						mesh.SetName(meshNode["Name"].as<std::string>());	//Mesh名

						YAML::Node subMeshes = meshNode["SubMeshes"];	//SubMeshes序列结点

						//SubMeshes序列结点存在
						if (subMeshes) {
							for (auto& subMeshNode : subMeshes) {
								YAML::Node verticesNode = subMeshNode["Vertices"];				//Vertices序列结点
								YAML::Node vertexIndicesNode = subMeshNode["VertexIndices"];	//VertexIndices序列结点

								std::vector<Vertex> vertices;	//顶点列表
								std::vector<uint32_t> indices;	//顶点索引列表

								//Vertices序列结点存在
								if (verticesNode) {
									for (auto& vertexNode : verticesNode) {
										//设置Vertex数据
										Vertex vertex;
										vertex.Position = vertexNode["Position"].as<glm::vec3>();
										vertex.Color = vertexNode["Color"].as<glm::vec4>();
										vertex.Normal = vertexNode["Normal"].as<glm::vec3>();
										vertex.TexCoord = vertexNode["TexCoord"].as<glm::vec2>();
										vertex.ID = vertexNode["ID"].as<int>();
										vertex.ObjectID = vertexNode["ObjectID"].as<int>();

										vertices.push_back(vertex);	//添加到顶点列表
									}
								}

								//VertexIndices序列结点存在 
								if (vertexIndicesNode) {
									for (auto& indexNode : vertexIndicesNode) {
										indices.push_back(indexNode["Index"].as<uint32_t>());	//添加顶点索引数据到列表
									}
								}

								mesh.AddSubMesh(SubMesh(vertices, indices));	//添加SubMesh到Mesh
							}
						}
					}
				}

				//Material组件结点
				auto materialNode = object["Material Component"];
				if (materialNode)
				{
					auto& material = deserializedObject.AddComponent<Material>();	//添加Material组件

					//TODO:添加Shader类型
					material.SetEnable(materialNode["Enable"].as<bool>());	//组件启用状态

					material.SetShader(ShaderLibrary::Get(materialNode["Shader"].as<std::string>()));	//加载Shader
					material.SetColor(materialNode["Color"].as<glm::vec4>());
					material.SetAlbedoTextureExist(materialNode["AlbedoTextureExist"].as<bool>());
					material.SetSpecularTextureExist(materialNode["SpecularTextureExist"].as<bool>());
					if (material.GetAlbedoTextureExist()) {		//Albedo存在
						material.SetAlbedoTexture(materialNode["AlbedoTexturePath"].as<std::string>());	//加载Albedo贴图
					}
					if (material.GetSpecularTextureExist()) {	//Specular存在
						material.SetSpecularTexture(materialNode["SpecularTexturePath"].as<std::string>());	//加载Specular贴图
					}
					material.SetShininess(materialNode["Shininess"].as<float>());
					material.SetTiling(materialNode["Tiling"].as<glm::vec2>());
					material.SetOffset(materialNode["Offset"].as<glm::vec2>());
				}

				//SpriteRenderer组件结点
				auto spriteRendererNode = object["SpriteRenderer Component"];
				if (spriteRendererNode)
				{
					auto& spriteRenderer = deserializedObject.AddComponent<SpriteRenderer>();	//添加SpriteRenderer组件

					spriteRenderer.SetEnable(spriteRendererNode["Enable"].as<bool>());	//组件启用状态
					spriteRenderer.SetColor(spriteRendererNode["Color"].as<glm::vec4>());
					//TODO待添加Sprite
				}
			}
		}

		//环境结点存在
		if (environment) {
			EXP_CORE_TRACE("Deserialized environment");

			Environment environm;	//环境对象
			//环境设置数据
			environm.SetSkyboxEnable(environment["SkyboxEnable"].as<bool>());	//天空盒启用状态
			environm.SetSourceType((Environment::SourceType)environment["SourceType"].as<int>());
			environm.SetIntensityMultiplier(environment["IntensityMultiplier"].as<float>());
			environm.SetAmbientColor(environment["AmbientColor"].as<glm::vec3>());

			//Skybox结点
			auto skyboxNode = environment["Skybox"];
			if (skyboxNode) {
				Skybox skybox;	//Skybox
				//设置天空盒数据
				skybox.SetShader(ShaderLibrary::Get(skyboxNode["Shader"].as<std::string>()));	//天空盒着色器
				skybox.SetTintColor(skyboxNode["TintColor"].as<glm::vec3>());
				skybox.SetExpose(skyboxNode["Expose"].as<float>());
				skybox.SetRotation(skyboxNode["Rotation"].as<float>());
				//Cubemap纹理路径
				std::string rightPath = skyboxNode["CubemapRightPath"].as<std::string>();
				std::string leftPath = skyboxNode["CubemapLeftPath"].as<std::string>();
				std::string upPath = skyboxNode["CubemapUpPath"].as<std::string>();
				std::string downPath = skyboxNode["CubemapDownPath"].as<std::string>();
				std::string frontPath = skyboxNode["CubemapFrontPath"].as<std::string>();
				std::string backPath = skyboxNode["CubemapBackPath"].as<std::string>();
				//设置Cubemap纹理
				if(rightPath.length() > 0) skybox.SetCubemapOneSideTexture(rightPath, Cubemap::TextureDirection::Right);
				if(leftPath.length() > 0) skybox.SetCubemapOneSideTexture(leftPath, Cubemap::TextureDirection::Left);
				if(upPath.length() > 0) skybox.SetCubemapOneSideTexture(upPath, Cubemap::TextureDirection::Up);
				if(downPath.length() > 0) skybox.SetCubemapOneSideTexture(downPath, Cubemap::TextureDirection::Down);
				if(frontPath.length() > 0) skybox.SetCubemapOneSideTexture(frontPath, Cubemap::TextureDirection::Front);
				if(backPath.length() > 0) skybox.SetCubemapOneSideTexture(backPath, Cubemap::TextureDirection::Back);

				environm.SetSkybox(skybox);	//设置环境的Skybox
			}
			m_Scene->SetEnvironment(environm);	//设置场景环境
		}

		return true;
	}
	
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//未实现
		return false;
	}
}