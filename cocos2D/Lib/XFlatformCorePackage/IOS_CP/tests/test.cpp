#include "SimpleApp/SimpleApp.h"
#include "Platform/Platform.h"
#include <iostream>

SimpleApp* g_app = NULL;

void OnPause();
void OnResume();
void OnRun();
void OnExit();

void EnableGeoLocation();
void DisableGeoLocation();

std::shared_ptr<platform::PlatformImpl> thePlatform;

void Main()
{
	g_app = SimpleApp::Create("iOSCorePackage Test");
	g_app->AddMenuItem("Enable GeoLocation", EnableGeoLocation);
	g_app->AddMenuItem("Disable GeoLocation", DisableGeoLocation);	
		
	g_app->SetPauseDelegate(std::bind(&OnPause));
	g_app->SetResumeDelegate(std::bind(&OnResume));
	g_app->SetRunDelegate(std::bind(&OnRun));
	g_app->SetExitDelegate(std::bind(&OnExit));
		
	platform::Settings settings;
	thePlatform = platform::PlatformImpl::CreateSingleInstance(settings);

	std::cout << "Country Code = " << thePlatform->GetDeviceInfo()->GetCountryCode() << std::endl;
    
	g_app->Run();
}

void EnableGeoLocation()
{
	thePlatform->GetGeoLocator()->Enable();
}

void DisableGeoLocation()
{
	thePlatform->GetGeoLocator()->Disable();
}

void OnRun()
{
}

void OnPause()
{
}

void OnResume()
{
}

void OnExit()
{
	delete g_app;
}