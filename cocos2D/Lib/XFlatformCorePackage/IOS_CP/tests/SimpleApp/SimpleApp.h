#ifndef __SIMPLEMENU_H__
#define __SIMPLEMENU_H__

#include <string>
#include <vector>
#include <map>
#include <functional>

class SimpleApp
{
public:
	static SimpleApp* Create(const std::string& name);	
	virtual ~SimpleApp() { }
	
	virtual void AddMenuItem(const std::string& text, std::function<void()> fn) = 0;
	virtual void Run() = 0;
	virtual void Alert(const std::string& message) = 0;

	void SetPauseDelegate(std::function<void()> pauseDelegate);
	void SetResumeDelegate(std::function<void()> resumeDelegate);
	void SetRunDelegate(std::function<void()> runDelegate);
	void SetExitDelegate(std::function<void()> exitDelegate);

protected:
	SimpleApp() {}

	std::function<void()> pauseDelegate;
	std::function<void()> resumeDelegate;
	std::function<void()> runDelegate;
	std::function<void()> exitDelegate;
};

#endif