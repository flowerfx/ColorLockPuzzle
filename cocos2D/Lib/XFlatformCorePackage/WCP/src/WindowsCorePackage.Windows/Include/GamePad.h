#pragma once

#pragma comment (lib , "xinput.lib")
#include <Xinput.h>

ref class CGamePad
{
internal:
	CGamePad();

	int m_nControllerID;
	XINPUT_STATE m_state;
	XINPUT_GAMEPAD *GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);

private:
	float m_fDeadzoneX;
	float m_fDeadzoneY;
	float m_fLeftStickX;
	float m_fLeftStickY;
	float m_fRightStickX;
	float m_fRightStickY;
	float m_fLeftTrigger;
	float m_fRightTrigger;
};