#include "NativeDispatcher.h"

extern void MainThreadDispatcherExecute();

@implementation NativeDispatcher

+(void) execute:(id)obj
{
	MainThreadDispatcherExecute();
}

@end