#ifndef __WIFIINFO_H__
#define __WIFIINFO_H__

/// This define (PLATFORM_IMPL) must be defined here because is a protection that 
/// that does not let you to include PlatformBaseInterface/PlatformBase.h directly.
#define PLATFORM_IMPL
#include "PlatformBaseInterface/PlatformBase.h"

#include <atomic>
#include <vector>
#include <algorithm>

namespace platform
{

	class WIFIInfo : public WIFIInfoBase
	{
		// Do not remove this friend directive. I know that this is not an 'by the book' approach, but solves the encapsulation problems.
		// Without this line the shared_ptr of template class PlatformImpl cannot access the private constructor & destructor.
		// If you remove this line, you need to make public the constructor & destructor => break the initial design => expose unneded/untested behaviour.
		friend class WIFIInfoBase;

	public:
		std::string GetInfoAsJson() override;
		void Enable() override;
		void Disable() override;
		bool IsEnabled() override;
		State GetState() override;
	private:
		WIFIInfo();

		
	#if (_MSC_VER < 1800) /// for VS before 2013 there is a bug that require public destructor for shared pointers.
	public:	
	#endif		
		~WIFIInfo();

	private:		
		/** This is used to store the current state */
		std::atomic<State> m_state;
        /** store the serialized json with wifi info */
		std::string m_data;
        /** info we can currently retrieve */
        static const std::vector<std::string> wifi_properties;
	};

} // namespace platform

#endif //__WIFIINFO_H__