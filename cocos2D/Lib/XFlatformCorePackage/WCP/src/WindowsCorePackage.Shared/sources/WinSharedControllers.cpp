#include "pch.h"

//controller support
//////////////////////////////////////////////////////////////////////////
/// set to 1 to enable XBOX 360 support for windows 8
//////////////////////////////////////////////////////////////////////////
#define WCP_XINPUT_SUPPORT 1
//////////////////////////////////////////////////////////////////////////
/// set to 1 to enable MOGA PRO support for windows phone 8
//////////////////////////////////////////////////////////////////////////
#define WCP_MOGAPRO_SUPPORT 1

#include "WinShared.h"


#if defined(OS_W8) && (WCP_XINPUT_SUPPORT == 1)
#include "GamePad.h" 
#endif

#if defined(OS_WP8)
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
#if (WCP_TEST_SAMPLE == 1)
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGamePadRefreshA()
	{
		std::string sStr = WCPToolkit::Controllers::GamePadRefreshA();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}
#endif
}
#endif

namespace WCPToolkit
{

#if defined(OS_W8) && (WCP_XINPUT_SUPPORT == 1)
	static CGamePad^ g_GamePad = nullptr;
	static Controllers::WCP_CONTROLLER_STATE g_ControllerState;
#elif defined(OS_WP8) && (WCP_MOGAPRO_SUPPORT)
	static Moga::Windows::Phone::ControllerManager^ g_GamePad = nullptr;
	static Controllers::WCP_CONTROLLER_STATE g_ControllerState;
#else 
	static void* g_GamePad = nullptr;
#endif
	static std::vector<Controllers::AppHidEventCallback> s_aHidCallbacks;

	void Controllers::RegisterEventCallback(Controllers::AppHidEventCallback pHidEventCallback)
	{
		s_aHidCallbacks.push_back(pHidEventCallback);
	}

	void Controllers::UnRegisterEventCallback(Controllers::AppHidEventCallback pHidEventCallback)
	{
		for (unsigned int idx = 0; idx < s_aHidCallbacks.size(); idx++)
		{
			if (pHidEventCallback == s_aHidCallbacks.at(idx))
			{
				s_aHidCallbacks.erase(s_aHidCallbacks.begin() + idx);
				break;
			}
		}
	}

#if (WCP_TEST_SAMPLE == 1)
	std::string Controllers::GamePadRefreshA()
	{
#if defined(OS_WP8) && (WCP_MOGAPRO_SUPPORT == 1)

		Controllers::GamePadRefresh();

		if (g_GamePad)
			if(((Moga::Windows::Phone::ControllerManager^)g_GamePad)->GetState(Moga::Windows::Phone::ControllerState::Connection) == Moga::Windows::Phone::ControllerResult::Connected)
			{
				if(g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::A) == Moga::Windows::Phone::ControllerAction::Pressed)
					return "TRUE";

				 //Platform::String^ sString = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::X).ToString();
				 //return std::string(sString->Begin(), sString->End());
			}
		return "FALSE";
#endif

		return WCP_NOT_AVAILABLE;
	}
#endif

	bool Controllers::GamePadRefresh()
	{
		bool result = true;
		
#if (defined(OS_W8) && (WCP_XINPUT_SUPPORT != 1)) || (defined(OS_WP8) && (WCP_MOGAPRO_SUPPORT != 1))
		result = false;
#endif
	//ID_CAP_PROXIMITY in WMAppManifest.xml
#if defined(OS_W8) && (WCP_XINPUT_SUPPORT == 1)
		if (!g_GamePad)
		{
			try
			{
				g_GamePad = ref new CGamePad;
			}
			catch (Platform::Exception^ e)
			{
				return false;
			}

			if (!g_GamePad)
				return false;
		}

		if (!g_GamePad->CheckConnection())
		{
			return false;
		}

		result = g_GamePad->Refresh();
#elif defined(OS_WP8) && (WCP_MOGAPRO_SUPPORT == 1)
		if (!g_GamePad)
		{
			try
			{
				g_GamePad = ref new Moga::Windows::Phone::ControllerManager();
				g_GamePad->Connect();
			}
			catch (Platform::Exception^ e)
			{
				return false;
			}

			if (!g_GamePad)
				return false;
		}
#endif

		if (s_aHidCallbacks.empty())
		{
			result = false;
		}
		else
		{
			bool bLocalResult = true;

			for (unsigned int idx = 0; idx < s_aHidCallbacks.size(); idx++)
			{
				bLocalResult = Controllers::GamePadCallback(s_aHidCallbacks.at(idx));
			}

#if defined(OS_WP8)
			result = bLocalResult;
#endif
		}

		return result;
	}

	bool Controllers::GamePadCallback(AppHidEventCallback pHidEventCallback)
	{
		bool result = false;

#if defined(OS_W8) && (WCP_XINPUT_SUPPORT == 1)
		double dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_A);
		if (dVal != g_ControllerState.wbsButtonA)
		{
			g_ControllerState.wbsButtonA = dVal;
			pHidEventCallback(ControllerEvents::ButtonA, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_B);
		if (dVal != g_ControllerState.wbsButtonB)
		{
			g_ControllerState.wbsButtonB = dVal;
			pHidEventCallback(ControllerEvents::ButtonB, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_X);
		if (dVal != g_ControllerState.wbsButtonX)
		{
			g_ControllerState.wbsButtonX = dVal;
			pHidEventCallback(ControllerEvents::ButtonX, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_Y);
		if (dVal != g_ControllerState.wbsButtonY)
		{
			g_ControllerState.wbsButtonY = dVal;
			pHidEventCallback(ControllerEvents::ButtonY, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_DPAD_UP);
		if (dVal != g_ControllerState.wbsDpadEventUp)
		{
			g_ControllerState.wbsDpadEventUp = dVal;
			pHidEventCallback(ControllerEvents::DpadEventUp, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_DPAD_DOWN);
		if (dVal != g_ControllerState.wbsDpadEventDown)
		{
			g_ControllerState.wbsDpadEventDown = dVal;
			pHidEventCallback(ControllerEvents::DpadEventDown, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT);
		if (dVal != g_ControllerState.wbsDpadEventLeft)
		{
			g_ControllerState.wbsDpadEventLeft = dVal;
			pHidEventCallback(ControllerEvents::DpadEventLeft, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT);
		if (dVal != g_ControllerState.wbsDpadEventRight)
		{
			g_ControllerState.wbsDpadEventRight = dVal;
			pHidEventCallback(ControllerEvents::DpadEventRight, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_START);
		if (dVal != g_ControllerState.wbsButtonStart)
		{
			g_ControllerState.wbsButtonStart = dVal;
			pHidEventCallback(ControllerEvents::ButtonStart, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_BACK);
		if (dVal != g_ControllerState.wbsButtonBack)
		{
			g_ControllerState.wbsButtonBack = dVal;
			pHidEventCallback(ControllerEvents::ButtonBack, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_LEFT_THUMB);
		if (dVal != g_ControllerState.wbsLeftStickButton)
		{
			g_ControllerState.wbsLeftStickButton = dVal;
			pHidEventCallback(ControllerEvents::LeftStickButton, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB);
		if (dVal != g_ControllerState.wbsRightStickButton)
		{
			g_ControllerState.wbsRightStickButton = dVal;
			pHidEventCallback(ControllerEvents::RightStickButton, dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER);
		if (dVal != g_ControllerState.wbsLeftBumper)
		{
			g_ControllerState.wbsLeftBumper = dVal;
			pHidEventCallback(ControllerEvents::LeftBumper,dVal);
		}
		dVal = g_GamePad->IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
		if (dVal != g_ControllerState.wbsRightBumper)
		{
			g_ControllerState.wbsRightBumper = dVal;
			pHidEventCallback(ControllerEvents::RightBumper, dVal);
		}
		dVal = g_GamePad->GetState()->bLeftTrigger / 255.0f;
		if (dVal != g_ControllerState.wbsLeftTrigger)
		{
			g_ControllerState.wbsLeftTrigger = dVal;
			pHidEventCallback(ControllerEvents::LeftTrigger, dVal);
		}
		dVal = g_GamePad->GetState()->bRightTrigger / 255.0f;
		if (dVal != g_ControllerState.wbsRightTrigger)
		{
			g_ControllerState.wbsRightTrigger = dVal;
			pHidEventCallback(ControllerEvents::RightTrigger, dVal);
		}
		dVal = g_GamePad->GetState()->sThumbLX / 32767.0f;
		if (dVal != g_ControllerState.wbsLeftStickX)
		{
			g_ControllerState.wbsLeftStickX = dVal;
			pHidEventCallback(ControllerEvents::LeftStickX, dVal);
		}
		dVal = g_GamePad->GetState()->sThumbLY / 32767.0f;
		if (dVal != g_ControllerState.wbsLeftStickY)
		{
			g_ControllerState.wbsLeftStickY = dVal;
			pHidEventCallback(ControllerEvents::LeftStickY, dVal);
		}
		dVal = g_GamePad->GetState()->sThumbRX / 32767.0f;
		//if (dVal != g_ControllerState.wbsRightStickX)
		{
			g_ControllerState.wbsRightStickX = dVal;
			pHidEventCallback(ControllerEvents::RightStickX, dVal);
		}
		dVal = g_GamePad->GetState()->sThumbRY / 32767.0f;
		//if (dVal != g_ControllerState.wbsRightStickY)
		{
			g_ControllerState.wbsRightStickY = dVal;
			pHidEventCallback(ControllerEvents::RightStickY, dVal);
		}
#elif defined(OS_WP8) && (WCP_MOGAPRO_SUPPORT == 1)
		if ((g_GamePad) && ((Moga::Windows::Phone::ControllerManager^)g_GamePad)->GetState(Moga::Windows::Phone::ControllerState::Connection) == Moga::Windows::Phone::ControllerResult::Connected)
		{
			result = true;

			double dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::A) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonA)
			{
				g_ControllerState.wbsButtonA = dVal;
				pHidEventCallback(ControllerEvents::ButtonA, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::B) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonB)
			{
				g_ControllerState.wbsButtonB = dVal;
				pHidEventCallback(ControllerEvents::ButtonB, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::X) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonX)
			{
				g_ControllerState.wbsButtonX = dVal;
				pHidEventCallback(ControllerEvents::ButtonX, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::Y) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonY)
			{
				g_ControllerState.wbsButtonY = dVal;
				pHidEventCallback(ControllerEvents::ButtonY, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::DirectionUp) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsDpadEventUp)
			{
				g_ControllerState.wbsDpadEventUp = dVal;
				pHidEventCallback(ControllerEvents::DpadEventUp, dVal);
			}			
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::DirectionDown) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsDpadEventDown)
			{
				g_ControllerState.wbsDpadEventDown = dVal;
				pHidEventCallback(ControllerEvents::DpadEventDown, dVal);
			}			
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::DirectionLeft) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsDpadEventLeft)
			{
				g_ControllerState.wbsDpadEventLeft = dVal;
				pHidEventCallback(ControllerEvents::DpadEventLeft, dVal);
			}			
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::DirectionRight) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsDpadEventRight)
			{
				g_ControllerState.wbsDpadEventRight = dVal;
				pHidEventCallback(ControllerEvents::DpadEventRight, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::Start) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonStart)
			{
				g_ControllerState.wbsButtonStart = dVal;
				pHidEventCallback(ControllerEvents::ButtonStart, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::Select) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsButtonSelect)
			{
				g_ControllerState.wbsButtonSelect = dVal;
				pHidEventCallback(ControllerEvents::ButtonSelect, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::ThumbLeft) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsLeftStickButton)
			{
				g_ControllerState.wbsLeftStickButton = dVal;
				pHidEventCallback(ControllerEvents::LeftStickButton, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::ThumbRight) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsRightStickButton)
			{
				g_ControllerState.wbsRightStickButton = dVal;
				pHidEventCallback(ControllerEvents::RightStickButton, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::L1) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsLeftBumper)
			{
				g_ControllerState.wbsLeftBumper = dVal;
				pHidEventCallback(ControllerEvents::LeftBumper, dVal);
			}
			dVal = g_GamePad->GetKeyCode(Moga::Windows::Phone::KeyCode::R1) == Moga::Windows::Phone::ControllerAction::Pressed;
			if (dVal != g_ControllerState.wbsRightBumper)
			{
				g_ControllerState.wbsRightBumper = dVal;
				pHidEventCallback(ControllerEvents::RightBumper, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::LeftTrigger);
			//events with an interval value should always trigger callback function
			{
				g_ControllerState.wbsLeftTrigger = dVal;
				pHidEventCallback(ControllerEvents::LeftTrigger, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::RightTrigger);
			//events with an interval value should always trigger callback function
			{
				g_ControllerState.wbsRightTrigger = dVal;
				pHidEventCallback(ControllerEvents::RightTrigger, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::X);
			if (dVal != g_ControllerState.wbsLeftStickX)
			{
				g_ControllerState.wbsLeftStickX = dVal;
				pHidEventCallback(ControllerEvents::LeftStickX, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::Y);
			if (dVal != g_ControllerState.wbsLeftStickY)
			{
				g_ControllerState.wbsLeftStickY = dVal;
				pHidEventCallback(ControllerEvents::LeftStickY, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::Z);
			//events with an interval value should always trigger callback function
			{
				g_ControllerState.wbsRightStickX = dVal;
				pHidEventCallback(ControllerEvents::RightStickX, dVal);
			}
			dVal = g_GamePad->GetAxisValue(Moga::Windows::Phone::Axis::RZ);
			//events with an interval value should always trigger callback function
			{
				g_ControllerState.wbsRightStickY = dVal;
				pHidEventCallback(ControllerEvents::RightStickY, dVal);
			}
		}
#endif

		return result;
	}
};