#pragma once

#include "MiniEngine/Renderer/OrthographicCamera.h"
#include "MiniEngine/Core/TimeStep.h"

#include "MiniEngine/Events/ApplicationEvent.h"	//resize
#include "MiniEngine/Events/MouseEvent.h"	//scorll

namespace MG
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);
		
		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		float GetZoomLevel() { return m_ZoomLevel; }


	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);

	private:
		float m_AspectRaion;	//纵横比
		float m_ZoomLevel = 1.0f;		//缩放级别
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraTranslationSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
	};
}