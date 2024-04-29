#include "SandBox2D.h"

SandBox2D::SandBox2D()
	: Layer("SandBox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void SandBox2D::OnAttach()
{
	m_Texture = MG::Texture2D::Create("assets/textures/PNG_test.png");

}

void SandBox2D::OnDetach()
{
}

void SandBox2D::OnUpdate(MG::TimeStep ts)
{
	//Update
	m_CameraController.OnUpdate(ts);


	//Render
	MG::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	MG::RenderCommand::Clear();


	MG::Renderer2D::BeginScene(m_CameraController.GetCamera());

	MG::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { m_SquareColor.r, m_SquareColor.g, m_SquareColor.b, 1.0f });
	MG::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.8f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });
	MG::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture);	//OpenGL使用右手系，z轴越小越在后面



	MG::Renderer2D::EndScene();





}

void SandBox2D::OnEvent(MG::Event& e)
{
	m_CameraController.OnEvent(e);
}

void SandBox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

	ImGui::End();
}
