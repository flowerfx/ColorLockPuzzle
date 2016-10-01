#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include "PlatformBaseInterface/PlatformBase.h"
#include "Platform/GeoLocator.h"
#include "Platform/DeviceInfo.h"
#include "Platform/FileSystem.h"

namespace platform
{

#if defined(WCP_DLL)
#define WINTK_EXPORTED __declspec(dllexport)
#else
#define WINTK_EXPORTED __declspec(dllimport)
#endif

	WINTK_EXPORTED class PlatformImpl : public BaseInterface<PlatformImpl, GeoLocator, DeviceInfo, FileSystem>
	{
		friend class BaseInterface < PlatformImpl, GeoLocator, DeviceInfo, FileSystem >;
	private:
		PlatformImpl() {}
	public:
		~PlatformImpl() {}
	};

} // pi

#endif