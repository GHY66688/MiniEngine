#pragma once

#include <MiniEngine.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

class SandBox2D : public MG::Layer
{
public:
	SandBox2D();

	virtual ~SandBox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnUpdate(MG::TimeStep ts) override;
	virtual void OnEvent(MG::Event& e) override;
	virtual void OnImGuiRender() override;

private:
	MG::OrthographicCameraController m_CameraController;

	MG::Ref<MG::VertexArray> m_VertexArray;
	MG::Ref<MG::Shader> m_Shader;
	MG::Ref<MG::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};