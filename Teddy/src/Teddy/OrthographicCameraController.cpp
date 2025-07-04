#include "teddyPch.h"
#include "OrthographicCameraController.h"

#include "Teddy/Input.h"
#include "Teddy/KeyCodes.h"

namespace Teddy
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(TED_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts.GetSeconds();

		else if (Input::IsKeyPressed(TED_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts.GetSeconds();

		if (Input::IsKeyPressed(TED_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts.GetSeconds();

		else if (Input::IsKeyPressed(TED_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts.GetSeconds();

		if (m_Rotation)
		{
			if (Input::IsKeyPressed(TED_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts.GetSeconds();
			if (Input::IsKeyPressed(TED_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts.GetSeconds();
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraTranslationSpeed = m_ZoomLevel;

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(TED_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(TED_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);

		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}
}