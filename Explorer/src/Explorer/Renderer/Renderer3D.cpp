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
		ShaderLibrary::Load("Circle", "assets/shaders/CircleShader");		//����Circle��ɫ��
		ShaderLibrary::Load("Line", "assets/shaders/LineShader");			//����Line��ɫ��
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��
		spriteShader->Bind();								//����ɫ��

		spriteShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����

		//-------------------Circle-----------------------------------
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle��ɫ��
		circleShader->Bind();								//����ɫ��

		circleShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����

		//-------------------Line-----------------------------------
		auto& lineShader = ShaderLibrary::Get("Line");	//Line��ɫ��
		lineShader->Bind();								//����ɫ��

		lineShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����
	}

	void Renderer3D::BeginScene(const Camera& camera, Transform& transform)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��
		spriteShader->Bind();	//����ɫ��

		glm::mat4& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform.GetTransform());	//����VP���� vp = p * v 

		spriteShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//����vp����

		//-------------------Circle-----------------------------------
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle��ɫ��
		circleShader->Bind();								//����ɫ��

		circleShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//����vp����

		//-------------------Line-----------------------------------
		auto& lineShader = ShaderLibrary::Get("Line");	//Line��ɫ��
		lineShader->Bind();								//����ɫ��

		lineShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//����vp����
	}

	void Renderer3D::DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, int objectID)
	{
		Sprite& sprite = spriteRenderer.GetSprite();

		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite��ɫ��

		spriteShader->Bind();			//����ɫ��

		sprite.GetTexture()->Bind();	//������

		//����uniform����
		spriteShader->SetInt("u_TextureExist", sprite.GetTextureExist());
		spriteShader->SetInt("u_Texture", 0);

		sprite.GetVertexBufferData().clear();	//�����һ�ζ��㻺��������
		//���ö�������
		for (int i = 0; i < 4; i++) {
			Vertex vertex = sprite.GetVertices()[i];

			vertex.Position = transform.GetTransform() * glm::vec4(vertex.Position, 1.0f);
			vertex.Color = spriteRenderer.GetColor();
			vertex.ObjectID = objectID;

			sprite.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
		}

		//SpriteRenderer���������
		if (spriteRenderer.GetEnable()) {
			Processing<Sprite, Vertex>(sprite, RenderCommandType::Triangle);	//��ȾSprite
		}

		s_Data.Stats.TriangleCount += 2;	//�����θ���
		s_Data.Stats.VertexCount += 4;		//�ۼƶ������
		s_Data.Stats.IndexCount += 6;		//�ۼƶ�����������
	}

	void Renderer3D::DrawCircle(const Transform& transform, CircleRenderer& circleRenderer, int objectID)
	{
		Circle& circle = circleRenderer.GetCircle();

		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle��ɫ��
		circleShader->Bind();								//����ɫ��

		circle.GetVertexBufferData().clear();	//�����һ�ζ��㻺��������
		//���ö�������
		for (int i = 0; i < 4; i++) {
			CircleVertex vertex = circle.GetVertices()[i];

			vertex.WorldPosition = transform.GetTransform() * glm::vec4(vertex.WorldPosition, 1.0f);
			vertex.Color = circle.GetColor();
			vertex.Thickness = circle.GetThickness();
			vertex.Fade = circle.GetFade();
			vertex.ObjectID = objectID;

			circle.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
		}

		//CircleRenderer���������
		if (circleRenderer.GetEnable()) {
			Processing<Circle, CircleVertex>(circle, RenderCommandType::Triangle);	//��ȾCircle
		}

		s_Data.Stats.TriangleCount += 2;	//�����θ���
		s_Data.Stats.VertexCount += 4;		//�ۼƶ������
		s_Data.Stats.IndexCount += 6;		//�ۼƶ�����������
	}

	void Renderer3D::DrawCircle(const glm::mat4& transform, Circle& circle, int objectID)
	{
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle��ɫ��
		circleShader->Bind();								//����ɫ��

		circle.GetVertexBufferData().clear();	//�����һ�ζ��㻺��������
		//���ö�������
		for (int i = 0; i < 4; i++) {
			CircleVertex vertex = circle.GetVertices()[i];

			vertex.WorldPosition = transform * glm::vec4(vertex.WorldPosition, 1.0f);
			vertex.Color = circle.GetColor();
			vertex.Thickness = circle.GetThickness();
			vertex.Fade = circle.GetFade();
			vertex.ObjectID = objectID;

			circle.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
		}

		Processing<Circle, CircleVertex>(circle, RenderCommandType::Triangle);	//��ȾCircle

		s_Data.Stats.TriangleCount += 2;	//�����θ���
		s_Data.Stats.VertexCount += 4;		//�ۼƶ������
		s_Data.Stats.IndexCount += 6;		//�ۼƶ�����������
	}

	void Renderer3D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID)
	{
		Line line(p0, p1, color, objectID);	//����ֱ��

		DrawLine(line);	//����ֱ��
	}

	void Renderer3D::DrawLine(Line& line)
	{
		auto& lineShader = ShaderLibrary::Get("Line");	//Line��ɫ��
		lineShader->Bind();								//����ɫ��

		RenderCommand::SetLineWidth(line.GetWidth());					//�����߿�
		Processing<Line, LineVertex>(line, RenderCommandType::Line);	//��ȾLine
	}

	void Renderer3D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int objectID)
	{
		glm::vec3 p0 = { position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z };	//����
		glm::vec3 p1 = { position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z };	//����
		glm::vec3 p2 = { position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };	//����
		glm::vec3 p3 = { position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z };	//����

		DrawLine(p0, p1, color, objectID);
		DrawLine(p1, p2, color, objectID);
		DrawLine(p2, p3, color, objectID);
		DrawLine(p3, p0, color, objectID);
	}

	void Renderer3D::DrawRect(const glm::mat4& transform, Rectangle& rectangle, int objectID)
	{
		auto& lineShader = ShaderLibrary::Get("Line");	//Line��ɫ��
		lineShader->Bind();								//����ɫ��

		rectangle.GetVertexBufferData().clear();	//�����һ�ζ��㻺��������
		//���ö�������
		for (int i = 0; i < 4; i++) {
			LineVertex vertex = rectangle.GetVertices()[i];

			vertex.Position = transform * glm::vec4(vertex.Position, 1.0f);
			vertex.Color = rectangle.GetColor();

			vertex.ObjectID = objectID;

			rectangle.GetVertexBufferData().push_back(vertex);	//��Ӷ��㻺��������
		}

		Processing<Rectangle, LineVertex>(rectangle, RenderCommandType::Rectangle);	//��ȾRectangle
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
				Processing<SubMesh, Vertex>(subMesh, RenderCommandType::Triangle);	//��Ⱦ������
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