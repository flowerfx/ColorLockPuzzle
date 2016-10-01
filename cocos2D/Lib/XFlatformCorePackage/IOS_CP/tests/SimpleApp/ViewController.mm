#import "ViewController.h"
#include "IOSSimpleApp.h"

@implementation ViewController

static IOSSimpleApp* s_app;

-(id) initWithApp:(id)app
{
	self = [super init]; 

	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appEnterBackground) name:UIApplicationDidEnterBackgroundNotification object:nil];	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appEnterForeground) name:UIApplicationWillEnterForegroundNotification object:nil];	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appEnterForegroundActive) name:UIApplicationDidBecomeActiveNotification object:nil];	
	[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(appEnterBackgroundActive) name:UIApplicationWillResignActiveNotification object:nil];
	
	s_app = (IOSSimpleApp*)app;
	
	return self;
}

-(void) close
{
	if (self.runTimer == nil)
		return;
    
	[self.runTimer invalidate];
	self.runTimer = nil;
}

-(void) didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
}

-(BOOL) prefersStatusBarHidden
{
	return YES;
}

- (BOOL)shouldAutorotate
{
	return YES;
}

-(NSUInteger) supportedInterfaceOrientations
{
	return UIInterfaceOrientationMaskAll;
    
//	return UIInterfaceOrientationMaskLandscape;
   
//  return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
}

-(void) appRun
{
	if (s_app != nil)
		s_app->OnRun();
}

-(void) appEnterBackgroundActive
{
	if (s_app != nil)
		s_app->OnPause();
}

-(void) appEnterForegroundActive
{
	if (s_app != nil)
		s_app->OnResume();
}

-(void) appEnterBackground
{
	if (s_app != nil)
		s_app->OnPause();
}

-(void) appEnterForeground
{
	if (s_app != nil)
		s_app->OnResume();
}

-(void) addButton:(UIButton*)button withText:(NSString*)text Y:(CGFloat)y
{
	[button addTarget:self action:@selector(buttonClicked:) forControlEvents:UIControlEventTouchUpInside];
	[button setTitle:text forState:UIControlStateNormal];
	button.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;

	button.backgroundColor = [UIColor colorWithRed:255.0/255.0 green:255.0/255.0 blue:255.0/255.0 alpha:1.0f];
	
	[button sizeToFit];
	
	CGRect screenSize = [[UIScreen mainScreen] bounds];
	int minValue = screenSize.size.width < screenSize.size.height ? screenSize.size.width : screenSize.size.height;
	int maxValue = screenSize.size.width > screenSize.size.height ? screenSize.size.width : screenSize.size.height;
	
	UIInterfaceOrientation  orientation = [[UIApplication sharedApplication] statusBarOrientation];
	if (UIInterfaceOrientationIsLandscape(orientation))
		button.center = CGPointMake(maxValue/2, y);
	else
		button.center = CGPointMake(minValue/2, y);

	[self.view addSubview:button]; 
}

-(void) buttonClicked:(UIButton*)sender
{
	s_app->ButtonClicked(sender);
}

@end
