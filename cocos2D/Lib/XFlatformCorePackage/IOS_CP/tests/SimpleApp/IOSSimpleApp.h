#ifndef __IOSSIMPLEAPP_H__
#define __IOSSIMPLEAPP_H__

#include "SimpleApp.h"
#include "ViewController.h"

#include <map>

class IOSSimpleApp : public SimpleApp
{
public:
	IOSSimpleApp(const std::string& name);
	~IOSSimpleApp();
	
	void OnRun()
	{
		if (runDelegate)
			runDelegate();
	}
	
	void OnPause()
	{
		if (pauseDelegate)
			pauseDelegate();
	}
	
	void OnResume()
	{
		if (resumeDelegate)
			resumeDelegate();
	}
	
	void ButtonClicked(UIButton* sender);

private:
	void AddMenuItem(const std::string& text, std::function<void()> fn);
	void Run();
	void Alert(const std::string& message);

	void Init();
	void Release();
	void RunLoop();
	void HandleAlert(NSString* messageString);
	void HandleAddButton(UIButton* button, NSString* textString, int y);	
	
private:
	ViewController* viewController;
	
	std::map<UIButton*, std::function<void()>> buttons;
	CGFloat currentMenuItemY;	
};

#endif