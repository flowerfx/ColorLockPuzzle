#include "Platform/Platform.h"
#include <Foundation/Foundation.h>

using namespace platform;

std::string PlatformImpl::GenerateUUID()
{
	CFUUIDRef uuid = CFUUIDCreate(kCFAllocatorDefault);
	NSString *uuidString = [(NSString *)CFUUIDCreateString(NULL, uuid) autorelease];
	CFRelease(uuid);

	return [uuidString UTF8String];
}