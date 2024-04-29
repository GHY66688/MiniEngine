#include "MGpch.h"

#include "OrthographicCameraController.h"

#include "MiniEngine/Core/Input.h"
#include "MiniEngine/Core/KeyCodes.h"

namespace MG
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRaion(aspectRatio), m_Camera(-m_AspectRaion * m_ZoomLevel, m_AspectRaion* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		if (Input::IsKeyPressed(MG_KEY_LEFT) || Input::IsKeyPressed(MG_KEY_A))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(MG_KEY_RIGHT) || Input::IsKeyPressed(MG_KEY_D))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		}

		if (Input::IsKeyPressed(MG_KEY_UP) || Input::IsKeyPressed(MG_KEY_W))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		}
		else if (Input::IsKeyPressed(MG_KEY_DOWN) || Input::IsKeyPressed(MG_KEY_S))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;
		}
		m_Camera.SetPosition(m_CameraPosition);

		//rotation
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(MG_KEY_Q))
			{
				m_CameraRotation -= m_CameraRotationSpeed * ts;
			}
			else if (Input::IsKeyPressed(MG_KEY_E))
			{
				m_CameraRotation += m_CameraRotationSpeed * ts;
			}

			m_Camera.SetRotation(m_CameraRotation);
		}

		//放大 移动慢，缩小 移动快
		m_CameraTranslationSpeed = m_ZoomLevel;


	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(MG_BIND_EVENT_FN(OrthographicCameraController::OnMouseScroll));
		dispatcher.Dispatch<WindowResizeEvent>(MG_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));

	}

	bool OrthographicCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRaion * m_ZoomLevel, m_AspectRaion * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRaion = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRaion * m_ZoomLevel, m_AspectRaion * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}