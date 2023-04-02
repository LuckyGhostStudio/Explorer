#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "RendererData.h"

#include "Texture.h"
#include "Explorer/Components/Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::s_Type = RendererType::Rasterization;

	/// <summary>
	/// ��Ⱦ������
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTextureSlotCount = 32;	//����������

		std::shared_ptr<ShaderLibrary> ShaderLibrary;	//��ɫ����
		std::shared_ptr<Shader>	MeshShader;				//��ɫ��
		std::shared_ptr<Texture2D>	WhiteTexture;		//��ɫ���� 0������

		uint32_t IndexCount = 0;						//��ǰ�Ѵ�������������

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//������б� �洢����
		uint32_t TextureSlotIndex = 1;					//��ǰ��������� 0 = white

		uint32_t MaxLightCount = 24;	//����Դ����
		uint32_t CurrentLightCount = 1;	//��ǰ��Դ����

		LightData LightData;			//��������
		MaterialData MaterialData;		//��������
		Renderer3D::Statistics Stats;	//ͳ������
	};

	static RendererData s_Data;	//��Ⱦ����

	void Renderer3D::Init()
	{
		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//�������Ϊ1������
		uint32_t whitTextureData = 0xffffffff;								//255��ɫ
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)

		int samplers[s_Data.MaxTextureSlotCount];		//��������� 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlotCount; i++) {
			samplers[i] = i;
		}

		s_Data.MeshShader = std::make_shared<Shader>("asserts/shaders/StandardShader");	//����Ĭ����ɫ��
		s_Data.MeshShader->Bind();														//����ɫ��
		//s_Data.MeshShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//����textures���� ���������

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0�������Ϊ��ɫ����

		//----------------------------------------------
		//s_Data.ShaderLibrary->Load("asserts/shaders/StandardShader");	//������ɫ��
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.GetTransform());	//vp = p * v

		s_Data.MeshShader->Bind();			//����ɫ��
		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//����vp����
		s_Data.MeshShader->SetFloat3("u_CameraPos", transform.m_Position);	//�������λ��
		s_Data.MeshShader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//���õƹ�λ��
		s_Data.MeshShader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//���õƹ���ɫ
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//���û�������ɫ
	}

	void Renderer3D::BeginScene(const EditorCamera& camera, std::vector<Object>& lightObjects)
	{
		//TODO:����ɫ����������ɫ����������
		s_Data.MeshShader->Bind();			//����ɫ��

		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����
		s_Data.MeshShader->SetFloat3("u_CameraPos", camera.GetPosition());		//�������λ��

		s_Data.CurrentLightCount = lightObjects.size();	//��ǰ��Դ����

		s_Data.MeshShader->SetInt("u_LightCount", s_Data.CurrentLightCount);	//��Դ����

		//����Light�����б�
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//��ԴTransform
			Light& light = lightObjects[i].GetComponent<Light>();					//��ԴLight

			//����Light����
			s_Data.LightData.Position = lightTransform.m_Position;				//��Դλ��
			s_Data.LightData.Direction = -lightTransform.GetForwardDirection();	//���շ��� z-
			s_Data.LightData.Color = light.m_Color;								//��Դ��ɫ
			s_Data.LightData.Intensity = light.GetIntensity();					//����ǿ��
			s_Data.LightData.RenderShadow = light.m_RenderShadow;				//�Ƿ���Ⱦ��Ӱ
			s_Data.LightData.Type = light.GetType();							//��������
			s_Data.LightData.Range = light.GetRange();							//���հ뾶
			s_Data.LightData.SpotOuterAngle = light.GetSpotOuterAngle();		//Spot���Ž�
			s_Data.LightData.SpotInnerAngle = light.GetSpotInnerAngle();		//Spot���Ž�

			//����Light uniform����
			s_Data.MeshShader->SetLightData(s_Data.LightData, i);
		}
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//���û�������ɫ
	}

	void Renderer3D::EndScene()
	{
		
	}

	void Renderer3D::SubMeshProcessing(SubMesh& subMesh)
	{
		uint32_t dataSize = (uint32_t)sizeof(Vertex) * subMesh.GetVertexBufferData().size();	//���㶥�㻺�������ݴ�С���ֽڣ�

		subMesh.GetVertexBuffer()->SetData(subMesh.GetVertexBufferData().data(), dataSize);	//���ö��㻺��������

		RenderCommand::DrawIndexed(subMesh.GetVertexArray());	//���Ƶ���

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}

	void Renderer3D::DrawMesh(const Transform& transform, Mesh& mesh, Material& material, int objectID)
	{
		std::shared_ptr<Shader>& shader = material.GetShader();
		
		material.BindTexture();	//������

		s_Data.MaterialData.Exist = true;												//�����Ƿ����:TODO:�ı�����Ըı�������������
		s_Data.MaterialData.Color = material.m_Color;									//��ɫ
		s_Data.MaterialData.AlbedoTextureExist = material.m_AlbedoTextureExist;			//��������ͼ�Ƿ����
		s_Data.MaterialData.SpecularTextureExist = material.m_SpecularTextureExist;		//�߹���ͼ�Ƿ����
		s_Data.MaterialData.AlbedoTextureSlot = (int)Material::TextureType::Albedo;		//��������ͼ��
		s_Data.MaterialData.SpecularTextureSlot = (int)Material::TextureType::Specular;	//�߹���ͼ��
		s_Data.MaterialData.Shininess = material.GetShininess();						//�����
		s_Data.MaterialData.Tiling = material.m_Tiling;									//����ƽ������
		s_Data.MaterialData.Offset = material.m_Offset;									//����ƫ����

		shader->SetMaterialData(s_Data.MaterialData);	//����Material����
		
		glm::mat4& transformMatrix = transform.GetTransform();

		//��������������
		for (SubMesh& subMesh : mesh) {
			subMesh.GetVertexBufferData().clear();	//�����һ��SubMesh���㻺��������
			//�������������ж���
			for (Vertex vertex : subMesh) {
				vertex.Position = transformMatrix * glm::vec4(vertex.Position, 1.0f);						//λ�� ��transform�任
				vertex.Normal = glm::mat3(glm::transpose(glm::inverse(transformMatrix))) * vertex.Normal;	//�������� M �任 Mȡ������ת�� ��ֹnormal������ʱ������
				vertex.ObjectID = objectID;

				subMesh.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
			}
			SubMeshProcessing(subMesh);		//��Ⱦ������
		}
		s_Data.IndexCount += mesh.GetVertexIndexCount();	//�ۼ�Mesh��������

		//s_Data.Stats.TriangleCount += 12;				//�����θ���
		s_Data.Stats.VertexCount += mesh.GetVertexCount();		//�ۼƶ������
		s_Data.Stats.IndexCount += mesh.GetVertexIndexCount();	//�ۼƶ�����������
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}