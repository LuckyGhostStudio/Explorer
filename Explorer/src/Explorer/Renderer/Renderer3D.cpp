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
		uint32_t MaxLightCount = 24;	//����Դ����
		uint32_t CurrentLightCount = 1;	//��ǰ��Դ����

		Renderer3D::Statistics Stats;	//ͳ������
	};

	static RendererData s_Data;	//��Ⱦ����

	void Renderer3D::Init()
	{
		ShaderLibrary::Load("Standard", "assets/shaders/StandardShader");	//���ر�׼��ɫ��
		ShaderLibrary::Load("Skybox", "assets/shaders/SkyboxShader");		//������պ���ɫ��
		ShaderLibrary::Load("Sprite", "assets/shaders/SpriteShader");		//����Sprite��ɫ��
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��
		spriteShader->Bind();	//����ɫ��

		spriteShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����
	}

	void Renderer3D::BeginScene(const Camera& camera, Transform& transform)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��
		spriteShader->Bind();	//����ɫ��

		glm::mat4& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform.GetTransform());	//����VP���� vp = p * v 

		spriteShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//����vp����
	}

	void Renderer3D::DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, int objectID)
	{
		Sprite& sprite = spriteRenderer.GetSprite();

		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��

		spriteShader->Bind();			//����ɫ��
		sprite.GetTexture()->Bind();	//������

		//����uniform����
		spriteShader->SetFloat4("u_Color", spriteRenderer.GetColor());
		spriteShader->SetInt("u_Texture", 0);

		sprite.GetVertexBufferData().clear();	//�����һ�ζ��㻺��������
		//���ö�������
		for (int i = 0; i < 4; i++) {
			Vertex vertex = sprite.GetVertices()[i];

			vertex.Position = transform.GetTransform() * glm::vec4(vertex.Position, 1.0f);
			vertex.ObjectID = objectID;

			sprite.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
		}

		//SpriteRenderer���������
		if (spriteRenderer.GetEnable()) {
			Processing<Sprite>(sprite);	//��ȾSprite
		}

		s_Data.Stats.TriangleCount += 2;	//�����θ���
		s_Data.Stats.VertexCount += 4;		//�ۼƶ������
		s_Data.Stats.IndexCount += 6;		//�ۼƶ�����������
	}

	void Renderer3D::BeginScene(Environment& environment, const Camera& camera, Transform& transform, std::vector<Object>& lightObjects)
	{
		s_Data.CurrentLightCount = lightObjects.size();	//��ǰ��Դ����

		auto& standardShader = ShaderLibrary::Get("Standard");		//��׼��ɫ��
		standardShader->Bind();	//�󶨱�׼��ɫ��

		glm::mat4& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform.GetTransform());	//����VP���� vp = p * v 

		standardShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//����vp����
		standardShader->SetFloat3("u_CameraPos", transform.GetPosition());			//�������λ��
		standardShader->SetInt("u_LightCount", s_Data.CurrentLightCount);			//��Դ����

		//����Light�����б�
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//��ԴTransform
			Light& light = lightObjects[i].GetComponent<Light>();					//��ԴLight

			//����Light Uniform����
			light.SetShaderData(lightTransform.GetPosition(), -lightTransform.GetForwardDirection(), standardShader, i);
		}

		//-------------------Skybox-----------------------------------
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

		//-------------------Skybox-----------------------------------
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

	void Renderer3D::EndScene(Environment& environment, const Camera& camera, Transform& transform)
	{
		//������պ�
		if (environment.GetSkyboxEnable()) {
			//������־ΪSkybox
			if (camera.GetClearFlag() == Camera::ClearFlag::Skybox) {
				glDepthFunc(GL_LEQUAL);	//TODO:ת�Ƶ�RenderCommand
				Skybox& skybox = environment.GetSkybox();	//��պ�
				auto& skyboxShader = skybox.GetShader();	//Skybox��ɫ��
				skyboxShader->Bind();						//��Skybox��ɫ��
				skybox.GetCubemap()->Bind();				//��Cubemap����

				glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//��ת������y�ᣩ
				glm::mat4& viewMatrix = glm::mat4(glm::mat3(glm::inverse(transform.GetTransform())));			//�Ƴ�view����λ�Ʋ��� ʹ�����Զ������պ�����

				skyboxShader->SetMat4("u_RotationMatrix", rotationMatrix);				//������ת����
				skyboxShader->SetMat4("u_ProjectionMatrix", camera.GetProjection());	//����projection����
				skyboxShader->SetMat4("u_ViewMatrix", viewMatrix);						//����view����
				skyboxShader->SetInt("u_Cubemap", 0);									//����Cubemap 0�Ų�λ
				skyboxShader->SetFloat3("u_TintColor", skybox.GetTintColor());			//����Skyboxɫ��
				skyboxShader->SetFloat("u_Expose", skybox.GetExpose());					//����Skybox�ع��

				RenderCommand::DrawIndexed(skybox.GetVertexArray());
				glDepthFunc(GL_LESS);	//TODO:ת�Ƶ�RenderCommand
			}
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

		s_Data.Stats.TriangleCount += mesh.GetVertexIndexCount() / 3;	//�����θ���
		s_Data.Stats.VertexCount += mesh.GetVertexCount();				//�ۼƶ��������ʵ�ʶ��������
		s_Data.Stats.IndexCount += mesh.GetVertexIndexCount();			//�ۼƶ�����������
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