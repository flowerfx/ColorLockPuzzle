#include "MainThreadDispatcher.h"
#include "NativeDispatcher.h"

#include <mutex>

static std::mutex g_mutex;
static std::function<void()> lambdaFunction;

void RunOnUiThread(std::function<void()> fn)
{
	g_mutex.lock();
	lambdaFunction = fn;
	// This will invoke MainThreadDispatcherExecute on the main thread
	[NativeDispatcher performSelectorOnMainThread:@selector(execute:) withObject:nil waitUntilDone:YES];
	g_mutex.unlock();
}

void MainThreadDispatcherExecute()
{
	lambdaFunction();
}