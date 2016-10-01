#include "IOSSimpleApp.h"
#include "ViewController.h"
#include "MainThreadDispatcher.h"
#include <thread>

#define MENU_Y_POSITION 50

IOSSimpleApp::IOSSimpleApp(const std::string& name)
{
	RunOnUiThread(std::bind(&IOSSimpleApp::Init, this));
}

void IOSSimpleApp::Init()
{
	CGRect bounds = [[UIScreen mainScreen] bounds];

	UIWindow* mainWindow = [[UIWindow alloc] initWithFrame:bounds];
	id appDelegate = [[UIApplication sharedApplication] delegate];

	[appDelegate performSelector:@selector(setWindow:) withObject:mainWindow];

	id app = (id)this;
	
	viewController = [[ViewController alloc] initWithApp:app];
	viewController.view = [[UIView alloc] initWithFrame:bounds];
	viewController.view.hidden = NO;
    
	[mainWindow setRootViewController: viewController];
	mainWindow.hidden = NO;
    
	currentMenuItemY = MENU_Y_POSITION;	
}

IOSSimpleApp::~IOSSimpleApp()
{
	RunOnUiThread(std::bind(&IOSSimpleApp::Release, this));
}

void IOSSimpleApp::Release()
{
	[viewController close];
}

void IOSSimpleApp::Run()
{
	std::thread* myThread = new std::thread(std::bind(&IOSSimpleApp::RunLoop, this));
	myThread->detach();	
}

void IOSSimpleApp::RunLoop()
{
	while (true)
	{
		OnRun();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void IOSSimpleApp::AddMenuItem(const std::string& text, std::function<void()> fn)
{
	NSString* textString = [NSString stringWithCString:text.c_str() encoding:[NSString defaultCStringEncoding]];
	
	UIButton *button = [UIButton buttonWithType:UIButtonTypeRoundedRect];	
	RunOnUiThread(std::bind(&IOSSimpleApp::HandleAddButton, this, button, textString, currentMenuItemY));
	buttons[button] = fn;	
	currentMenuItemY += (int)(button.frame.size.height * 1.3f);
}

void IOSSimpleApp::HandleAddButton(UIButton* button, NSString* textString, int y)
{
	[viewController addButton:button withText:textString Y:currentMenuItemY];
}

void IOSSimpleApp::Alert(const std::string& message)
{
	NSString* messageString = [NSString stringWithCString:message.c_str() encoding:[NSString defaultCStringEncoding]];
	RunOnUiThread(std::bind(&IOSSimpleApp::HandleAlert, this, messageString));
}

void IOSSimpleApp::HandleAlert(NSString* messageString)
{
	UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Info" message:messageString delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
	[alert show];
	[alert autorelease];	
}

void IOSSimpleApp::ButtonClicked(UIButton* button)
{
	buttons[button]();
}