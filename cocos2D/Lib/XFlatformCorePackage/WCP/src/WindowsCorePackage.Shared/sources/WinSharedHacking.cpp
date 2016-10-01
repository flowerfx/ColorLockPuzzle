#include "pch.h"

#include "WinShared.h"


#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
#include "AntiHack.h"
#endif

#if defined(OS_WP81)
#include "client.h"
#endif

#ifdef OS_W10
#include "wrl/client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTrueFalseString()
	{
		return TrueFalseString();
	}
	bool WCPToolkitWinPhone80CppCallbacks::nativeTrueFalseBool()
	{
		return TrueFalseBool();
	}

}
#endif


namespace WCPToolkit
{
	std::wstring TrueFalse::TrueFalseString()
	{
		std::wstring sStr;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		sStr = AntiHack::AntiHackChecker::GetInstance()->GetTestString();
#elif defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeTrueFalseString)();
		sStr = std::wstring(sString->Begin(), sString->End());
#endif

		return sStr;
	}

	bool TrueFalse::TrueFalseBool()
	{
		bool bRet = false;

#if defined(OS_W8)
#elif defined(OS_WP8)
		bRet = NATIVE_CALL(nativeTrueFalseBool)();
#endif

		return bRet;
	}

};