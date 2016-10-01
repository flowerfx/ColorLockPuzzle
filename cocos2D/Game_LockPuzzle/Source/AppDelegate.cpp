#include "AppDelegate.h"

#include <vector>
#include <string>

#include "Game/MainGame.h"

#include "DisplayView.h"
#if USE_CC_AUDIO
#include "SoundManager.h"
#endif

#include "AppMacros.h"

USING_NS_CC;
using namespace std;
float scaleFactorTouch = 1.0f;
AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#ifdef _WIN32
		glview = GLViewImpl::createWithRect("Game Server Test cc3.9", Rect(0, 0, 540, 960));
#else
		glview = GLViewImpl::create("Game Server Test cc3.9");
#endif
        director->setOpenGLView(glview);
    }
	designResolutionSize = cocos2d::Size(540, 960);
    director->setOpenGLView(glview);
#if defined WP8 || defined OS_W8 || defined OS_ANDROID || defined OS_IOS || defined _WIN32
	designResolutionSize = glview->getDesignResolutionSize();
#endif
    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

	cocos2d::Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;

    // In this demo, we select resource according to the frame's height.
    // If the resource size is different from design resolution size, you need to set contentScaleFactor.
    // We use the ratio of resource's height to the height of design resolution,
    // this can make sure that the resource's height could fit for the height of design resolution.

    // if the frame's height is larger than the height of medium resource size, select large resource.
	//if (frameSize.height > mediumResource.size.height)
	{
#ifdef _WIN32
		searchPath.push_back("../../../../DataPacked/Data");
#else
        searchPath.push_back(largeResource.directory);
#endif

 //       director->setContentScaleFactor(MIN(largeResource.size.height/designResolutionSize.height, largeResource.size.width/designResolutionSize.width));
	}
   
    
    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
#if defined _DEBUG || defined DEBUG
	director->setDisplayStats(true);
#else
	director->setDisplayStats(false);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	// run
	MainGame::GetInstance()->InitForFirstScence(director);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
