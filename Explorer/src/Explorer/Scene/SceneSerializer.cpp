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
//TODO:���Mesh���л��ͷ����л�
//TODO:���Material���л��ͷ����л�
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

		//Name���
		if (object.HasComponent<Name>()){
			out << YAML::Key << "Name Component";
			out << YAML::BeginMap;	//��ʼName���Map
			
			auto& name = object.GetComponent<Name>().m_Name;	//Name�������
			out << YAML::Key << "Name" << YAML::Value << name;

			out << YAML::EndMap;	//����Name���Map
		}

		//Transform���
		if (object.HasComponent<Transform>()) {
			out << YAML::Key << "Transform Component";
			out << YAML::BeginMap;	//��ʼTransform���Map

			//Transform�������
			auto& transform = object.GetComponent<Transform>();

			out << YAML::Key << "Position" << YAML::Value << transform.m_Position;
			out << YAML::Key << "Rotation" << YAML::Value << transform.m_Rotation;
			out << YAML::Key << "Scale" << YAML::Value << transform.m_Scale;

			out << YAML::EndMap;	//����Transform���Map
		}

		//Camera���
		if (object.HasComponent<Camera>())
		{
			out << YAML::Key << "Camera Component";
			out << YAML::BeginMap; //��ʼCamera���Map

			auto& camera = object.GetComponent<Camera>();

			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "Size" << YAML::Value << camera.GetSize();
			out << YAML::Key << "FOV" << YAML::Value << camera.GetFOV();
			out << YAML::Key << "Near" << YAML::Value << camera.GetNearClip();
			out << YAML::Key << "Far" << YAML::Value << camera.GetFarClip();
			out << YAML::Key << "Primary" << YAML::Value << camera.m_Primary;

			out << YAML::EndMap; //����Camera���Map
		}

		//Light���
		if (object.HasComponent<Light>())
		{
			out << YAML::Key << "Light Component";
			out << YAML::BeginMap; //��ʼLight���Map

			auto& light = object.GetComponent<Light>();

			out << YAML::Key << "Type" << YAML::Value << (int)light.GetType();
			out << YAML::Key << "Range" << YAML::Value << light.GetRange();
			out << YAML::Key << "SpotOuterAngle" << YAML::Value << light.GetSpotOuterAngle();
			out << YAML::Key << "SpotInnerAngle" << YAML::Value << light.GetSpotInnerAngle();
			out << YAML::Key << "Color" << YAML::Value << light.m_Color;
			out << YAML::Key << "Intensity" << YAML::Value << light.GetIntensity();
			out << YAML::Key << "RenderShadow" << YAML::Value << light.m_RenderShadow;

			out << YAML::EndMap; //����Light���Map
		}
		
		//Mesh���
		if (object.HasComponent<Mesh>())
		{
			out << YAML::Key << "Mesh Component";
			out << YAML::BeginMap;	//��ʼMesh���Map

			auto& mesh = object.GetComponent<Mesh>();
			out << YAML::Key << "Type" << YAML::Value << (int)mesh.GetType();
			//TODO:�����������

			out << YAML::EndMap;	//����Mesh���Map
		}

		//Material���
		if (object.HasComponent<Material>())
		{
			out << YAML::Key << "Material Component";
			out << YAML::BeginMap;	//��ʼMaterial���Map

			auto& material = object.GetComponent<Material>();
			
			//TODO:���Shader����
			out << YAML::Key << "Color" << YAML::Value << material.m_Color;									//��ɫ
			out << YAML::Key << "AlbedoTextureExist" << YAML::Value << material.m_AlbedoTextureExist;		//Albedo�Ƿ����
			out << YAML::Key << "SpecularTextureExist" << YAML::Value << material.m_SpecularTextureExist;	//Specular�Ƿ����								//��ɫ
			if (material.m_AlbedoTextureExist) {
				out << YAML::Key << "AlbedoTexturePath" << YAML::Value << material.GetAlbedoTexture()->GetPath();		//Albedo��ͼ·��
			}
			if (material.m_SpecularTextureExist) {
				out << YAML::Key << "SpecularTexturePath" << YAML::Value << material.GetSpecularTexture()->GetPath();	//Specular��ͼ·��
			}
			out << YAML::Key << "Shininess" << YAML::Value << material.GetShininess();						//�����
			out << YAML::Key << "Tiling" << YAML::Value << material.m_Tiling;								//����ƽ������
			out << YAML::Key << "Offset" << YAML::Value << material.m_Offset;								//����ƽ������

			out << YAML::EndMap;	//����Material���Map
		}

		//SpriteRenderer���
		if (object.HasComponent<SpriteRenderer>())
		{
			out << YAML::Key << "SpriteRenderer Component";
			out << YAML::BeginMap;	//��ʼSpriteRenderer���Map

			auto& spriteRenderer = object.GetComponent<SpriteRenderer>();
			out << YAML::Key << "Color" << YAML::Value << spriteRenderer.m_Color;

			out << YAML::EndMap;	//����SpriteRenderer���Map
		}

		out << YAML::EndMap;	//��������Map
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;		//������
		out << YAML::BeginMap;	//��ʼ����Map
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";			//������������
		out << YAML::Key << "Objects" << YAML::Value << YAML::BeginSeq;		//���壺��ʼ��������
		m_Scene->m_Registry.each([&](auto objectID)		//��������ע�����������
		{
			Object object = { objectID, m_Scene.get() };
			if (!object)
				return;

			SerializeObject(out, object);			//���л�����
		});
		out << YAML::EndSeq;	//������������
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
		EXP_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		YAML::Node objects = data["Objects"];	//�������н��

		if (objects) {	//���������
			for (auto object : objects) {	//�����������������
				uint64_t uuid = object["Object"].as<uint64_t>(); //����uid TODO

				std::string name;
				auto nameNode = object["Name Component"];	//Name������
				if (nameNode) {
					name = nameNode["Name"].as<std::string>();	//������
				}

				EXP_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Object deserializedObject = m_Scene->CreateEmptyObject(name);	//�����Ŀ�����

				//Transform������
				auto transformNode = object["Transform Component"];
				if (transformNode){
					auto& transform = deserializedObject.GetComponent<Transform>();	//��ȡTransform���
					//���ñ任�������
					transform.m_Position = transformNode["Position"].as<glm::vec3>();
					transform.m_Rotation = transformNode["Rotation"].as<glm::vec3>();
					transform.m_Scale = transformNode["Scale"].as<glm::vec3>();
				}

				//Camera������
				auto cameraNode = object["Camera Component"];
				if (cameraNode)
				{
					auto& camera = deserializedObject.AddComponent<Camera>();	//���Camera���

					//��������������
					camera.SetProjectionType((Camera::ProjectionType)cameraNode["ProjectionType"].as<int>());
					camera.SetSize(cameraNode["Size"].as<float>());
					camera.SetFOV(cameraNode["FOV"].as<float>());
					camera.SetNearClip(cameraNode["Near"].as<float>());
					camera.SetFarClip(cameraNode["Far"].as<float>());
					camera.m_Primary = cameraNode["Primary"].as<bool>();
				}

				//Light������
				auto lightNode = object["Light Component"];
				if (lightNode)
				{
					auto& light = deserializedObject.AddComponent<Light>();	//���Light���

					//���ù�Դ�������
					light.SetType((Light::Type)lightNode["Type"].as<int>());
					light.SetRange(lightNode["Range"].as<float>());
					light.SetSpotOuterAngle(lightNode["SpotOuterAngle"].as<float>());
					light.SetSpotInnerAngle(lightNode["SpotInnerAngle"].as<float>());
					light.m_Color = lightNode["Color"].as<glm::vec3>();
					light.SetIntensity(lightNode["Intensity"].as<float>());
					light.m_RenderShadow = lightNode["RenderShadow"].as<bool>();
				}

				//Mesh������
				auto meshNode = object["Mesh Component"];
				if (meshNode)
				{
					auto& mesh = deserializedObject.AddComponent<Mesh>();	//���Mesh���

					//���������������
					mesh.SetType((Mesh::Type)meshNode["Type"].as<int>());

					std::vector<Vertex> vertices;	//Mesh����
					std::vector<uint32_t> indices;	//Mesh��������

					switch (mesh.GetType())
					{
						case Mesh::Type::None:
							break;
						case Mesh::Type::Other:
							break;
						case Mesh::Type::Cube:
							//�����嶥��
							vertices =
							{
								// ---------λ��---------	----------��ɫ----------		---------����--------	---��������---  ID objID
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

							//��������
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

							mesh.AddSubMesh(SubMesh(vertices, indices));	//���Cube������
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
					//TODO:��������
				}

				//Material������
				auto materialNode = object["Material Component"];
				if (materialNode)
				{
					auto& material = deserializedObject.AddComponent<Material>();	//���Material���

					//���ù�Դ�������
					//TODO:���Shader����
					material.m_Color = materialNode["Color"].as<glm::vec4>();
					material.m_AlbedoTextureExist = materialNode["AlbedoTextureExist"].as<bool>();
					material.m_SpecularTextureExist = materialNode["SpecularTextureExist"].as<bool>();
					if (material.m_AlbedoTextureExist) {	//Albedo����
						material.SetAlbedoTexture(materialNode["AlbedoTexturePath"].as<std::string>());	//����Albedo��ͼ
					}
					if (material.m_SpecularTextureExist) {	//Specular����
						material.SetSpecularTexture(materialNode["SpecularTexturePath"].as<std::string>());	//����Specular��ͼ
					}
					material.SetShininess(materialNode["Shininess"].as<float>());
					material.m_Tiling = materialNode["Tiling"].as<glm::vec2>();
					material.m_Offset = materialNode["Offset"].as<glm::vec2>();
				}

				//SpriteRenderer������
				auto spriteRendererNode = object["SpriteRenderer Component"];
				if (spriteRendererNode)
				{
					auto& spriteRenderer = deserializedObject.AddComponent<SpriteRenderer>();	//���SpriteRenderer���
					spriteRenderer.m_Color = spriteRendererNode["Color"].as<glm::vec4>();
				}
			}
		}

		return true;
	}
	
	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EXP_CORE_ASSERT(false, "Not implemented!");	//δʵ��
		return false;
	}
}