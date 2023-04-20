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
	/// vec2ת��
	/// </summary>
	template<>
	struct convert<glm::vec2>
	{
		/// <summary>
		/// ��vec2ת��ΪYAML�Ľڵ�
		/// </summary>
		/// <param name="rhs">vec2����</param>
		/// <returns>���</returns>
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		/// <summary>
		/// ��YAML�������ת��Ϊvec2
		/// </summary>
		/// <param name="node">���</param>
		/// <param name="rhs">vec2</param>
		/// <returns>�Ƿ�ת���ɹ�</returns>
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
	/// vec3ת��
	/// </summary>
	template<>
	struct convert<glm::vec3>
	{
		/// <summary>
		/// ��vec3ת��ΪYAML�Ľڵ�
		/// </summary>
		/// <param name="rhs">vec3����</param>
		/// <returns>���</returns>
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		/// <summary>
		/// ��YAML�������ת��Ϊvec3
		/// </summary>
		/// <param name="node">���</param>
		/// <param name="rhs">vec3</param>
		/// <returns>�Ƿ�ת���ɹ�</returns>
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
	/// vec4ת��
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
		out << YAML::Flow;	//�� [x,y]
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;	//�� [x,y,z]
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;	//�� [x,y,z,w]
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene> scene) :m_Scene(scene)
	{

	}

	/// <summary>
	/// ���л�����
	/// </summary>
	/// <param name="out">������</param>
	/// <param name="object">����</param>
	static void SerializeObject(YAML::Emitter& out, Object object)
	{
		out << YAML::BeginMap;	//��ʼ����Map
		out << YAML::Key << "Object" << YAML::Value << "12837192831273"; // TODO: Object ID goes here

		//Self���
		if (object.HasComponent<Self>()){
			out << YAML::Key << "Self Component";
			out << YAML::BeginMap;	//��ʼSelf���Map
			
			auto& self = object.GetComponent<Self>();	//Self�������
			out << YAML::Key << "ObjectName" << YAML::Value << self.GetObjectName();		//������
			out << YAML::Key << "ObjectEnable" << YAML::Value << self.GetObjectEnable();	//��������״̬

			out << YAML::EndMap;	//����Self���Map
		}

		//Transform���
		if (object.HasComponent<Transform>()) {
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;	//��ʼTransform���Map

			//Transform�������
			auto& transform = object.GetComponent<Transform>();

			out << YAML::Key << "Position" << YAML::Value << transform.GetPosition();
			out << YAML::Key << "Rotation" << YAML::Value << transform.GetRotation();
			out << YAML::Key << "Scale" << YAML::Value << transform.GetScale();

			out << YAML::EndMap;	//����Transform���Map
		}

		//Camera���
		if (object.HasComponent<Camera>())
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap; //��ʼCamera���Map

			auto& camera = object.GetComponent<Camera>();

			out << YAML::Key << "Enable" << YAML::Value << camera.GetEnable();

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::Key << "Primary" << YAML::Value << camera.IsPrimary();

			out << YAML::EndMap; //����Camera���Map
		}

		//Light���
		if (object.HasComponent<Light>())
		{
			out << YAML::Key << "Light Component";
			out << YAML::BeginMap; //��ʼLight���Map

			auto& light = object.GetComponent<Light>();

			out << YAML::Key << "Enable" << YAML::Value << light.GetEnable();

			out << YAML::Key << "Type" << YAML::Value << (int)light.GetType();
			out << YAML::Key << "Range" << YAML::Value << light.GetRange();
			out << YAML::Key << "SpotOuterAngle" << YAML::Value << light.GetSpotOuterAngle();
			out << YAML::Key << "SpotInnerAngle" << YAML::Value << light.GetSpotInnerAngle();
			out << YAML::Key << "Color" << YAML::Value << light.GetColor();
			out << YAML::Key << "Intensity" << YAML::Value << light.GetIntensity();
			out << YAML::Key << "RenderShadow" << YAML::Value << light.GetCastShadow();

			out << YAML::EndMap; //����Light���Map
		}
		
		//Mesh���
		if (object.HasComponent<Mesh>())
		{
			out << YAML::Key << "Mesh Component";
			out << YAML::BeginMap;	//��ʼMesh���Map

			auto& mesh = object.GetComponent<Mesh>();

			out << YAML::Key << "Enable" << YAML::Value << mesh.GetEnable();

			out << YAML::Key << "Type" << YAML::Value << (int)mesh.GetType();

			//��������Mesh����
			if (mesh.GetType() == Mesh::Type::Other) {
				out << YAML::Key << "Name" << YAML::Value << mesh.GetName();

				out << YAML::Key << "SubMeshes" << YAML::Value << YAML::BeginSeq;	//�����񣺿�ʼSubMesh����
			
				//��������������
				for (int i = 0; i < mesh.GetSubMeshCount(); i++) {
					SubMesh& subMesh = mesh.GetSubMeshes()[i];

					out << YAML::BeginMap;			//��ʼSubMesh Map
					out << YAML::Key << "SubMesh" << YAML::Value << i;	//SubMesh���

					out << YAML::Key << "Vertices" << YAML::Value << YAML::BeginSeq;	//���㣺��ʼVertex����
					
					//��������
					for (int j = 0; j < subMesh.GetVertexCount(); j++) {
						Vertex& vertex = subMesh.GetVertices()[j];

						out << YAML::BeginMap;			//��ʼVertex Map
						out << YAML::Key << "Vertex" << YAML::Value << j;	//Vertex���

						out << YAML::Key << "Position" << YAML::Value << vertex.Position;
						out << YAML::Key << "Color" << YAML::Value << vertex.Color;
						out << YAML::Key << "Normal" << YAML::Value << vertex.Normal;
						out << YAML::Key << "TexCoord" << YAML::Value << vertex.TexCoord;
						out << YAML::Key << "ID" << YAML::Value << vertex.ID;
						out << YAML::Key << "ObjectID" << YAML::Value << vertex.ObjectID;

						out << YAML::EndMap;			//����Vertex Map
					}

					out << YAML::EndSeq;												//����Vertex����

					out << YAML::Key << "VertexIndices" << YAML::Value << YAML::BeginSeq;	//������������ʼVertexIndices����

					//������������
					for (int j = 0; j < subMesh.GetVertexIndexCount(); j++) {
						out << YAML::BeginMap;									//��ʼVertexIndex Map
						out << YAML::Key << "VertexIndex" << YAML::Value << j;	//VertexIndex���

						out << YAML::Key << "Index" << YAML::Value << subMesh.GetVertexIndices()[j];

						out << YAML::EndMap;				//����VertexIndex Map
					}

					out << YAML::EndSeq;												//����VertexIndices����

					out << YAML::EndMap;			//����SubMesh Map
				}
				out << YAML::EndSeq;												//����SubMesh����
			}

			out << YAML::EndMap;	//����Mesh���Map
		}

		//Material���
		if (object.HasComponent<Material>())
		{
			out << YAML::Key << "Material Component";
			out << YAML::BeginMap;	//��ʼMaterial���Map

			auto& material = object.GetComponent<Material>();
			
			out << YAML::Key << "Enable" << YAML::Value << material.GetEnable();

			out << YAML::Key << "Shader" << YAML::Value << material.GetShader()->GetName();	//Shader��

			out << YAML::Key << "Color" << YAML::Value << material.GetColor();								//��ɫ
			out << YAML::Key << "AlbedoTextureExist" << YAML::Value << material.GetAlbedoTextureExist();	//Albedo�Ƿ����
			out << YAML::Key << "SpecularTextureExist" << YAML::Value << material.GetSpecularTextureExist();//Specular�Ƿ����								//��ɫ
			if (material.GetAlbedoTextureExist()) {
				out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material.GetAlbedoTexture()->GetPath();		//Albedo��ͼ·��
			}
			if (material.GetSpecularTextureExist()) {
				out << YAML::Key << "SpecularTexturePath" << YAML::Value << material.GetSpecularTexture()->GetPath();	//Specular��ͼ·��
			}
			out << YAML::Key << "Shininess" << YAML::Value << material.GetShininess();						//�����
			out << YAML::Key << "Tiling" << YAML::Value << material.GetTiling();							//����ƽ������
			out << YAML::Key << "Offset" << YAML::Value << material.GetOffset();							//����ƽ������

			out << YAML::EndMap;	//����Material���Map
		}

		//SpriteRenderer���
		if (object.HasComponent<SpriteRenderer>())
		{
			out << YAML::Key << "SpriteRenderer Component";
			out << YAML::BeginMap;	//��ʼSpriteRenderer���Map

			auto& spriteRenderer = object.GetComponent<SpriteRenderer>();

			out << YAML::Key << "Enable" << YAML::Value << spriteRenderer.GetEnable();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.GetColor();
			//TODO:�����Sprite

			out << YAML::EndMap;	//����SpriteRenderer���Map
		}

		out << YAML::EndMap;	//��������Map
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;		//������
		out << YAML::BeginMap;	//��ʼ����Map
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->GetName();	//������������

		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;		//���壺��ʼ��������
		m_Scene->m_Registry.each([&](auto objectID)		//��������ע�����������
		{
			Object object = { objectID, m_Scene.get() };
			if (!object)
				return;

			SerializeObject(out, object);			//���л�����
		});
		out << YAML::EndSeq;	//������������

		out << YAML::Key << "Environment";	//�������ý��
		out << YAML::BeginMap;		//��ʼ����Map

		Environment& environment = m_Scene->GetEnvironment();	//��������
		//������������
		out << YAML::Key << "SkyboxEnable" << YAML::Value << environment.GetSkyboxEnable();		//��պ�����״̬
		out << YAML::Key << "SourceType" << YAML::Value << (int)environment.GetSourceType();	
		out << YAML::Key << "IntensityMultiplier" << YAML::Value << environment.GetIntensityMultiplier();
		out << YAML::Key << "AmbientColor" << YAML::Value << environment.GetAmbientColor();
		
		out << YAML::Key << "Skybox";	//Skybox���
		out << YAML::BeginMap;			//��ʼSkyboxMap
		Skybox& skybox = environment.GetSkybox();
		//��պв���
		out << YAML::Key << "Shader" << YAML::Value << skybox.GetShader()->GetName();	//��ɫ����
		out << YAML::Key << "TintColor" << YAML::Value << skybox.GetTintColor();
		out << YAML::Key << "Expose" << YAML::Value << skybox.GetExpose();
		out << YAML::Key << "Rotation" << YAML::Value << skybox.GetRotation();
		//Cubemap ����·��
		out << YAML::Key << "CubemapRightPath" << YAML::Value << skybox.GetPreviewTextures()[0]->GetPath();
		out << YAML::Key << "CubemapLeftPath" << YAML::Value << skybox.GetPreviewTextures()[1]->GetPath();
		out << YAML::Key << "CubemapUpPath" << YAML::Value << skybox.GetPreviewTextures()[2]->GetPath();
		out << YAML::Key << "CubemapDownPath" << YAML::Value << skybox.GetPreviewTextures()[3]->GetPath();
		out << YAML::Key << "CubemapFrontPath" << YAML::Value << skybox.GetPreviewTextures()[4]->GetPath();
		out << YAML::Key << "CubemapBackPath" << YAML::Value << skybox.GetPreviewTextures()[5]->GetPath();

		out << YAML::EndMap;			//����Skybox Map

		out << YAML::EndMap;		//��������Map

		out << YAML::EndMap;	//��������Map

		std::ofstream fout(filepath);	//�����
		fout << out.c_str();			//������л������������ļ�
	}
	
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//δʵ��
	}
	
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);	//�ļ�������
		std::stringstream strStream;
		strStream << stream.rdbuf();	//�ļ������ַ���

		YAML::Node data = YAML::Load(strStream.str());	//���ص�YMAL���
		//Scene�ڵ㲻����
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();	//������
		m_Scene->SetName(sceneName);

		EXP_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		YAML::Node objects = data["Objects"];			//�������н��
		YAML::Node environment = data["Environment"];	//�������ý��

		if (objects) {	//���������
			for (auto object : objects) {	//�����������������
				uint64_t uuid = object["Object"].as<uint64_t>(); //����uid TODO

				std::string objectName;
				bool objectEnable;
				auto selfNode = object["Self Component"];	//Self������
				if (selfNode) {
					objectName = selfNode["ObjectName"].as<std::string>();	//������
					objectEnable = selfNode["ObjectEnable"].as<bool>();		//��������״̬
				}

				EXP_CORE_TRACE("Deserialized object with ID = {0}, name = {1}", uuid, objectName);

				Object deserializedObject = m_Scene->CreateEmptyObject(objectName, objectEnable);	//�����Ŀ�����

				//Transform������
				auto transformNode = object["Transform Component"];
				if (transformNode){
					auto& transform = deserializedObject.GetComponent<Transform>();	//��ȡTransform���
					
					//���ñ任�������
					transform.SetPosition(transformNode["Position"].as<glm::vec3>());
					transform.SetRotation(transformNode["Rotation"].as<glm::vec3>());
					transform.SetScale(transformNode["Scale"].as<glm::vec3>());
				}

				//Camera������
				auto cameraNode = object["Camera Component"];
				if (cameraNode)
				{
					auto& camera = deserializedObject.AddComponent<Camera>();	//���Camera���

					//��������������
					camera.SetEnable(cameraNode["Enable"].as<bool>());	//�������״̬

					camera.SetProjectionType((Camera::ProjectionType)cameraNode["ProjectionType"].as<int>());
					camera.SetSize(cameraNode["Size"].as<float>());
					camera.SetFOV(cameraNode["FOV"].as<float>());
					camera.SetNearClip(cameraNode["Near"].as<float>());
					camera.SetFarClip(cameraNode["Far"].as<float>());
					camera.SetPrimary(cameraNode["Primary"].as<bool>());
				}

				//Light������
				auto lightNode = object["Light Component"];
				if (lightNode)
				{
					auto& light = deserializedObject.AddComponent<Light>();	//���Light���

					//���ù�Դ�������
					light.SetEnable(lightNode["Enable"].as<bool>());	//�������״̬

					light.SetType((Light::Type)lightNode["Type"].as<int>());
					light.SetRange(lightNode["Range"].as<float>());
					light.SetSpotOuterAngle(lightNode["SpotOuterAngle"].as<float>());
					light.SetSpotInnerAngle(lightNode["SpotInnerAngle"].as<float>());
					light.SetColor(lightNode["Color"].as<glm::vec3>());
					light.SetIntensity(lightNode["Intensity"].as<float>());
					light.SetCastShadow(lightNode["RenderShadow"].as<bool>());
				}

				//Mesh������
				auto meshNode = object["Mesh Component"];
				if (meshNode)
				{
					auto& mesh = deserializedObject.AddComponent<Mesh>();	//���Mesh���

					//���������������
					mesh.SetEnable(meshNode["Enable"].as<bool>());	//�������״̬

					mesh.SetType((Mesh::Type)meshNode["Type"].as<int>());

					//��������Mesh����
					if (mesh.GetType() == Mesh::Type::Other) {
						mesh.SetName(meshNode["Name"].as<std::string>());	//Mesh��

						YAML::Node subMeshes = meshNode["SubMeshes"];	//SubMeshes���н��

						//SubMeshes���н�����
						if (subMeshes) {
							for (auto& subMeshNode : subMeshes) {
								YAML::Node verticesNode = subMeshNode["Vertices"];				//Vertices���н��
								YAML::Node vertexIndicesNode = subMeshNode["VertexIndices"];	//VertexIndices���н��

								std::vector<Vertex> vertices;	//�����б�
								std::vector<uint32_t> indices;	//���������б�

								//Vertices���н�����
								if (verticesNode) {
									for (auto& vertexNode : verticesNode) {
										//����Vertex����
										Vertex vertex;
										vertex.Position = vertexNode["Position"].as<glm::vec3>();
										vertex.Color = vertexNode["Color"].as<glm::vec4>();
										vertex.Normal = vertexNode["Normal"].as<glm::vec3>();
										vertex.TexCoord = vertexNode["TexCoord"].as<glm::vec2>();
										vertex.ID = vertexNode["ID"].as<int>();
										vertex.ObjectID = vertexNode["ObjectID"].as<int>();

										vertices.push_back(vertex);	//��ӵ������б�
									}
								}

								//VertexIndices���н����� 
								if (vertexIndicesNode) {
									for (auto& indexNode : vertexIndicesNode) {
										indices.push_back(indexNode["Index"].as<uint32_t>());	//��Ӷ����������ݵ��б�
									}
								}

								mesh.AddSubMesh(SubMesh(vertices, indices));	//���SubMesh��Mesh
							}
						}
					}
				}

				//Material������
				auto materialNode = object["Material Component"];
				if (materialNode)
				{
					auto& material = deserializedObject.AddComponent<Material>();	//���Material���

					//TODO:���Shader����
					material.SetEnable(materialNode["Enable"].as<bool>());	//�������״̬

					material.SetShader(ShaderLibrary::Get(materialNode["Shader"].as<std::string>()));	//����Shader
					material.SetColor(materialNode["Color"].as<glm::vec4>());
					material.SetAlbedoTextureExist(materialNode["AlbedoTextureExist"].as<bool>());
					material.SetSpecularTextureExist(materialNode["SpecularTextureExist"].as<bool>());
					if (material.GetAlbedoTextureExist()) {		//Albedo����
						material.SetAlbedoTexture(materialNode["AlbedoTexturePath"].as<std::string>());	//����Albedo��ͼ
					}
					if (material.GetSpecularTextureExist()) {	//Specular����
						material.SetSpecularTexture(materialNode["SpecularTexturePath"].as<std::string>());	//����Specular��ͼ
					}
					material.SetShininess(materialNode["Shininess"].as<float>());
					material.SetTiling(materialNode["Tiling"].as<glm::vec2>());
					material.SetOffset(materialNode["Offset"].as<glm::vec2>());
				}

				//SpriteRenderer������
				auto spriteRendererNode = object["SpriteRenderer Component"];
				if (spriteRendererNode)
				{
					auto& spriteRenderer = deserializedObject.AddComponent<SpriteRenderer>();	//���SpriteRenderer���

					spriteRenderer.SetEnable(spriteRendererNode["Enable"].as<bool>());	//�������״̬
					spriteRenderer.SetColor(spriteRendererNode["Color"].as<glm::vec4>());
					//TODO�����Sprite
				}
			}
		}

		//����������
		if (environment) {
			EXP_CORE_TRACE("Deserialized environment");

			Environment environm;	//��������
			//������������
			environm.SetSkyboxEnable(environment["SkyboxEnable"].as<bool>());	//��պ�����״̬
			environm.SetSourceType((Environment::SourceType)environment["SourceType"].as<int>());
			environm.SetIntensityMultiplier(environment["IntensityMultiplier"].as<float>());
			environm.SetAmbientColor(environment["AmbientColor"].as<glm::vec3>());

			//Skybox���
			auto skyboxNode = environment["Skybox"];
			if (skyboxNode) {
				Skybox skybox;	//Skybox
				//������պ�����
				skybox.SetShader(ShaderLibrary::Get(skyboxNode["Shader"].as<std::string>()));	//��պ���ɫ��
				skybox.SetTintColor(skyboxNode["TintColor"].as<glm::vec3>());
				skybox.SetExpose(skyboxNode["Expose"].as<float>());
				skybox.SetRotation(skyboxNode["Rotation"].as<float>());
				//Cubemap����·��
				std::string rightPath = skyboxNode["CubemapRightPath"].as<std::string>();
				std::string leftPath = skyboxNode["CubemapLeftPath"].as<std::string>();
				std::string upPath = skyboxNode["CubemapUpPath"].as<std::string>();
				std::string downPath = skyboxNode["CubemapDownPath"].as<std::string>();
				std::string frontPath = skyboxNode["CubemapFrontPath"].as<std::string>();
				std::string backPath = skyboxNode["CubemapBackPath"].as<std::string>();
				//����Cubemap����
				if(rightPath.length() > 0) skybox.SetCubemapOneSideTexture(rightPath, Cubemap::TextureDirection::Right);
				if(leftPath.length() > 0) skybox.SetCubemapOneSideTexture(leftPath, Cubemap::TextureDirection::Left);
				if(upPath.length() > 0) skybox.SetCubemapOneSideTexture(upPath, Cubemap::TextureDirection::Up);
				if(downPath.length() > 0) skybox.SetCubemapOneSideTexture(downPath, Cubemap::TextureDirection::Down);
				if(frontPath.length() > 0) skybox.SetCubemapOneSideTexture(frontPath, Cubemap::TextureDirection::Front);
				if(backPath.length() > 0) skybox.SetCubemapOneSideTexture(backPath, Cubemap::TextureDirection::Back);

				environm.SetSkybox(skybox);	//���û�����Skybox
			}
			m_Scene->SetEnvironment(environm);	//���ó�������
		}

		return true;
	}
	
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//δʵ��
		return false;
	}
}