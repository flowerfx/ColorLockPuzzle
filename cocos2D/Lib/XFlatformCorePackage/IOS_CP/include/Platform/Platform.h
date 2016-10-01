#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/// This define (PLATFORM_IMPL) must be defined here because is a protection that 
/// that does not let you to include PlatformBaseInterface/PlatformBase.h directly.
#define PLATFORM_IMPL
#include "PlatformBaseInterface/PlatformBase.h"

#include "Platform/GeoLocator.h"
#include "Platform/DeviceInfo.h"
#include "Platform/FileSystem.h"
#include "Platform/WIFIInfo.h"

namespace platform
{
	class PlatformImpl : public PlatformBaseInterface
	{
		// Do not remove this friend directive. I know that this is not an 'by the book' approach, but solves the encapsulation problems.
		// Without this line the shared_ptr of template class PlatformImpl cannot access the private constructor & destructor.
		// If you remove this line, you need to make public the constructor & destructor => break the initial design => expose unneded/untested behaviour.
		friend class PlatformBaseInterface;

	public:
		std::string GenerateUUID() override;

	private:
		PlatformImpl() {}
		~PlatformImpl() {}		
	};
} // platform

#endif