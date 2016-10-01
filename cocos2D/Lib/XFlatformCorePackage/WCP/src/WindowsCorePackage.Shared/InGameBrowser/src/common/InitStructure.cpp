#include "pch.h"

#include "InitStructure.h"
#include "..\win8.1\InGameBrowser.h"

namespace WCPToolkit
{
    namespace InGameBrowserLibrary
    {
			InitParams::InitParams()
            {
                Language = nullptr;
				AnonymousAccount = nullptr;
				FacebookId = nullptr;
				GLiveAccount = nullptr;
				GameCode = nullptr;
				GameOperator = nullptr;
				GameVersion = nullptr;
				HDIDFV = nullptr;
				BaseUrl = nullptr;
				ClientID = nullptr;
				WAID = nullptr;
				XboxID = nullptr;
                UserAge = -1;
            }


			bool InitParams::CheckParams()
            {
                bool isValid = true;

				int languageIndex = -1;
				for (int i = 0; i < sizeof(TXT_IGB_LANGUAGES) / sizeof(*TXT_IGB_LANGUAGES); ++i)
				{
					if (TXT_IGB_LANGUAGES[i]->Equals(Language))
					{
						languageIndex = i;
						break;
					}
				}

                if (languageIndex == -1)
                {
#if defined(_DEBUG)
                   __debugbreak(); //Please set a valid language index
#endif
                    Language = "EN";
                    isValid = false;
                }

                if (AnonymousAccount == nullptr || AnonymousAccount->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid Anonymous Account
#endif
					AnonymousAccount = ref new Platform::String();
                    isValid = false;
                }

				if (FacebookId == nullptr)
                {   //Not mandatory
					FacebookId = ref new Platform::String();
                }

				if (GLiveAccount == nullptr)
                {   //Not mandatory
					GLiveAccount = ref new Platform::String();
                }

				if (GameCode == nullptr || GameCode->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid Game Code
#endif
					GameCode = ref new Platform::String();
                    isValid = false;
                }

				if (GameOperator == nullptr || GameOperator->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid Game Operator
#endif
					GameOperator = ref new Platform::String();
                    isValid = false;
                }

				if (HDIDFV == nullptr || HDIDFV->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid HDIDFV
#endif
					HDIDFV = ref new Platform::String();
                    isValid = false;
                }

				if (BaseUrl == nullptr || BaseUrl->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid Base Url
#endif
					BaseUrl = ref new Platform::String();
                    isValid = false;
                }

				if (ClientID == nullptr || ClientID->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid Client ID
#endif
					ClientID = ref new Platform::String();
                    isValid = false;
                }

				if (WAID == nullptr || WAID->IsEmpty())
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid WAID
#endif
					WAID = ref new Platform::String();
                    isValid = false;
                }

				if (XboxID == nullptr)
                {   //Not mandatory
					XboxID = ref new Platform::String();
                }

                if (UserAge <= 0)
                {
#if defined(_DEBUG)
					__debugbreak(); //Please set a valid User Age
#endif
                    UserAge = 0;
                    isValid = false;
                }

                return isValid;
            }
    }
}