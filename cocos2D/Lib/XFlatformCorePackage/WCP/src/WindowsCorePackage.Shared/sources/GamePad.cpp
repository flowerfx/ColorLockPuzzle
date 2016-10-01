#include "pch.h"

#include "../include/GamePad.h"
#include <cmath>

CGamePad::CGamePad()
{
	m_fDeadzoneX = 0.05f;
	m_fDeadzoneY = 0.02f;
	m_fLeftStickX = 0.0f;
	m_fLeftStickY = 0.0f;
	m_fRightStickX = 0.0f;
	m_fRightStickY = 0.0f;
	m_fLeftTrigger = 0.0f;
	m_fRightTrigger = 0.0f;
	m_nControllerID = -1;
};

XINPUT_GAMEPAD *CGamePad::GetState()
{
	return &m_state.Gamepad;
}

bool CGamePad::CheckConnection()
{
	int controllerId = -1;

	for (DWORD idx = 0; (idx < XUSER_MAX_COUNT) && (controllerId == -1); idx++)
	{
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		if (XInputGetState(idx, &state) == ERROR_SUCCESS)
			controllerId = idx;
	}

	m_nControllerID = controllerId;

	return controllerId != -1;
}

// Returns false if the controller has been disconnected
bool CGamePad::Refresh()
{
	if (m_nControllerID == -1)
		CheckConnection();

	if (m_nControllerID != -1)
	{
		ZeroMemory(&m_state, sizeof(XINPUT_STATE));
		if (XInputGetState(m_nControllerID, &m_state) != ERROR_SUCCESS)
		{
			m_nControllerID = -1;
			return false;
		}

		float normLX = fmaxf(-1, (float)m_state.Gamepad.sThumbLX / 32767);
		float normLY = fmaxf(-1, (float)m_state.Gamepad.sThumbLY / 32767);

		m_fLeftStickX = (abs(normLX) < m_fDeadzoneX ? 0 : (abs(normLX) - m_fDeadzoneX) * (normLX / abs(normLX)));
		m_fLeftStickY = (abs(normLY) < m_fDeadzoneY ? 0 : (abs(normLY) - m_fDeadzoneY) * (normLY / abs(normLY)));

		if (m_fDeadzoneX > 0) 
			m_fLeftStickX *= 1 / (1 - m_fDeadzoneX);
		if (m_fDeadzoneY > 0) 
			m_fLeftStickY *= 1 / (1 - m_fDeadzoneY);

		float normRX = fmaxf(-1, (float)m_state.Gamepad.sThumbRX / 32767);
		float normRY = fmaxf(-1, (float)m_state.Gamepad.sThumbRY / 32767);

		m_fRightStickX = (abs(normRX) < m_fDeadzoneX ? 0 : (abs(normRX) - m_fDeadzoneX) * (normRX / abs(normRX)));
		m_fRightStickY = (abs(normRY) < m_fDeadzoneY ? 0 : (abs(normRY) - m_fDeadzoneY) * (normRY / abs(normRY)));

		if (m_fDeadzoneX > 0) 
			m_fRightStickX *= 1 / (1 - m_fDeadzoneX);
		if (m_fDeadzoneY > 0) 
			m_fRightStickY *= 1 / (1 - m_fDeadzoneY);

		m_fLeftTrigger = (float)m_state.Gamepad.bLeftTrigger / 255;
		m_fRightTrigger = (float)m_state.Gamepad.bRightTrigger / 255;

		return true;
	}

	return false;
}

bool CGamePad::IsPressed(WORD button)
{
	//XINPUT_GAMEPAD_A
	//XINPUT_GAMEPAD_B
	//XINPUT_GAMEPAD_X
	//XINPUT_GAMEPAD_Y
	//Directional pad arrows

	//XINPUT_GAMEPAD_DPAD_LEFT
	//XINPUT_GAMEPAD_DPAD_RIGHT
	//XINPUT_GAMEPAD_DPAD_UP
	//XINPUT_GAMEPAD_DPAD_DOWN
	//Shoulder buttons

	//XINPUT_GAMEPAD_LEFT_SHOULDER
	//XINPUT_GAMEPAD_RIGHT_SHOULDER
	//Analog thumb sticks(when pressed in and used as a button)

	//XINPUT_GAMEPAD_LEFT_THUMB
	//XINPUT_GAMEPAD_RIGHT_THUMB
	//Center buttons

	//XINPUT_GAMEPAD_BACK
	//XINPUT_GAMEPAD_START

	return (m_state.Gamepad.wButtons & button) != 0;
}