#include "MainMenuScene.h"

#include <Teddy.h>

namespace Cuphead
{
	bool MainMenuScene::OnDlcMenuKeyPressed(Teddy::KeyPressedEvent& e)
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