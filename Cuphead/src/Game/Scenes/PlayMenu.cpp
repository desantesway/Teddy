#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	bool MainMenuScene::OnPlayMenuKeyPressed(Teddy::KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case Teddy::Key::Escape:
			m_CurrentMenu = 0;
			UpdateMainMenuButtonColors();
			return true;
		default:
			break;
		}

		return false;
	}
}