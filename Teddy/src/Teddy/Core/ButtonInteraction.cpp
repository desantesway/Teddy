#include "Teddy/Core/ButtonInteraction.h"

#include "Teddy/Core/Input.h"
#include "Teddy/PostProcessing/PostProcessing.h"

namespace Teddy
{
    entt::entity ButtonInteractionSystem::m_HoveredEntity = entt::null;
    uint32_t ButtonInteractionSystem::m_Width = 1920, ButtonInteractionSystem::m_Height = 1080;
	ButtonInteractionSystem* ButtonInteractionSystem::s_Instance = nullptr;
    bool ButtonInteractionSystem::m_Enabled = true;

    ButtonInteractionSystem::ButtonInteractionSystem()
    {
		TED_ASSERT(!s_Instance, "ButtonInteractionSystem already exists!");
		s_Instance = this;
    }

	void ButtonInteractionSystem::OnUpdate()
	{
        if (m_Enabled)
        {
            glm::vec2 mouse = Input::GetMousePosition();
            int mouseX = mouse.x;
            int mouseY = mouse.y;

            TED_CORE_INFO("Mouse Position: {0}, {1}", mouseX, mouseY);

            mouseY = m_Height - mouseY;

            if (mouseX >= 0 && mouseY >= 0 && mouseX < m_Width && mouseY < m_Height)
            {
                int pixelData = PostProcessing::GetFramebuffer()->ReadPixel(1, mouseX, mouseY);

                if (pixelData == -1)
                {
                    m_HoveredEntity = entt::null;
                }
                else
                {
                    m_HoveredEntity = (entt::entity)pixelData;
                }
            }
        }
	}

    void ButtonInteractionSystem::OnWindowResize(uint32_t width, uint32_t height)
    {
		m_Width = width;
		m_Height = height;
    }
}