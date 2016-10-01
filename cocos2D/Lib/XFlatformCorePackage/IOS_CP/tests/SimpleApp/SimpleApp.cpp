#include "SimpleApp.h"
#include "IOSSimpleApp.h"

SimpleApp* SimpleApp::Create(const std::string& name)
{
	return new IOSSimpleApp(name);
}

void SimpleApp::SetPauseDelegate(std::function<void()> pauseDelegate)
{
	this->pauseDelegate = pauseDelegate;
}

void SimpleApp::SetResumeDelegate(std::function<void()> resumeDelegate)
{
	this->resumeDelegate = resumeDelegate;
}

void SimpleApp::SetRunDelegate(std::function<void()> runDelegate)
{
	this->runDelegate = runDelegate;
}

void SimpleApp::SetExitDelegate(std::function<void()> exitDelegate)
{
	this->exitDelegate = exitDelegate;
}