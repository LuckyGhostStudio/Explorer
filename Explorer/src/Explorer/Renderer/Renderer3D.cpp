#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Texture.h"
#include "Explorer/Components/Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>	//TODO:���Ƴ�

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::s_Type = RendererType::Rasterization;

	/// <summary>
	/// ��Ⱦ������
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTextureSlotCount = 32;	//����������

		std::shared_ptr<Texture2D>	WhiteTexture;		//��ɫ���� 0������

		uint32_t IndexCount = 0;						//��ǰ�Ѵ�������������

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//������б� �洢����
		uint32_t TextureSlotIndex = 1;					//��ǰ��������� 0 = white

		uint32_t MaxLightCount = 24;	//����Դ����
		uint32_t CurrentLightCount = 1;	//��ǰ��Դ����

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

		//s_Data.MeshShader = std::make_shared<Shader>("asserts/shaders/StandardShader");	//����Ĭ����ɫ��
		//s_Data.MeshShader->Bind();														//����ɫ��
		//s_Data.MeshShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//����textures���� ���������

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0�������Ϊ��ɫ����

		//----------------------------------------------
		ShaderLibrary::Load("Standard", "asserts/shaders/StandardShader");	//���ر�׼��ɫ��
		ShaderLibrary::Load("Skybox", "asserts/shaders/SkyboxShader");		//������պ���ɫ��
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const Camera& camera, Transform& transform)
	{
		if (camera.GetEnable()) {

		}
	}

	void Renderer3D::BeginScene(Environment& environment, const EditorCamera& camera, std::vector<Object>& lightObjects)
	{
		s_Data.CurrentLightCount = lightObjects.size();	//��ǰ��Դ����

		auto& standardShader = ShaderLibrary::Get("Standard");		//��׼��ɫ��
		standardShader->Bind();	//�󶨱�׼��ɫ��

		standardShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����
		standardShader->SetFloat3("u_CameraPos", camera.GetPosition());							//�������λ��
		standardShader->SetInt("u_LightCount", s_Data.CurrentLightCount);						//��Դ����

		//����Light�����б�
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//��ԴTransform
			Light& light = lightObjects[i].GetComponent<Light>();					//��ԴLight
			
			//����Light Uniform����
			light.SetShaderData(lightTransform.GetPosition(), -lightTransform.GetForwardDirection(), standardShader, i);
		}

		Skybox& skybox = environment.GetSkybox();	//��պ�
		skybox.GetCubemap()->Bind();				//��Cubemap����
		glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//Skybox��ת������y�ᣩ
		
		standardShader->SetInt("u_Environment.EnableSkybox", environment.GetSkyboxEnable());	//���� �Ƿ�������պ�
		standardShader->SetInt("u_Environment.Cubemap", 0);										//����Cubemap
		standardShader->SetFloat3("u_Environment.TintColor", skybox.GetTintColor());			//����Skyboxɫ��
		standardShader->SetFloat("u_Environment.Expose", skybox.GetExpose());					//����Skybox�ع��
		standardShader->SetMat4("u_Environment.SkyboxRotationMatrix", rotationMatrix);			//����Skybox��ת������y�ᣩ
		standardShader->SetInt("u_Environment.SourceType", (int)environment.GetSourceType());	//���û�����Դ����
		standardShader->SetFloat("u_Environment.IntensityMultiplier", environment.GetIntensityMultiplier());	//������պй�ǿ����
		standardShader->SetFloat3("u_Environment.AmbientColor", environment.GetAmbientColor());	//���û�������ɫ
	}

	void Renderer3D::EndScene(Environment& environment, const EditorCamera& camera)
	{
		//������պ�
		if (environment.GetSkyboxEnable()) {
			glDepthFunc(GL_LEQUAL);	//TODO:ת�Ƶ�RenderCommand
			Skybox& skybox = environment.GetSkybox();	//��պ�
			auto& skyboxShader = skybox.GetShader();	//Skybox��ɫ��
			skyboxShader->Bind();						//��Skybox��ɫ��
			skybox.GetCubemap()->Bind();				//��Cubemap����

			glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//��ת������y�ᣩ
			glm::mat4& viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));		//�Ƴ�view����λ�Ʋ��� ʹ�����Զ������պ�����

			skyboxShader->SetMat4("u_RotationMatrix", rotationMatrix);					//������ת����
			skyboxShader->SetMat4("u_ProjectionMatrix", camera.GetProjectionMatrix());	//����projection����
			skyboxShader->SetMat4("u_ViewMatrix", viewMatrix);							//����view����
			skyboxShader->SetInt("u_Cubemap", 0);										//����Cubemap 0�Ų�λ
			skyboxShader->SetFloat3("u_TintColor", skybox.GetTintColor());				//����Skyboxɫ��
			skyboxShader->SetFloat("u_Expose", skybox.GetExpose());						//����Skybox�ع��

			RenderCommand::DrawIndexed(skybox.GetVertexArray());
			glDepthFunc(GL_LESS);	//TODO:ת�Ƶ�RenderCommand
		}
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
		material.SetShaderData();	//����Material��Shader Uniform����

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

			//Mesh�������
			if (mesh.GetEnable()) {
				SubMeshProcessing(subMesh);		//��Ⱦ������
			}
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