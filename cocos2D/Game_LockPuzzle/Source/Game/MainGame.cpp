#include "MainGame.h"
#include "InputManager.h"
#if USE_CC_AUDIO
#include "SoundManager.h"
#endif
#include "FXFlatform/XFlatform.h" //for the cross flatform wp8/android/ios
#include "ObjectTexture.h"

#include "Menu/MenuManager.h"
#include "Effect.h"
#include "BoostMgr/BoostManager.h"

#include "FileManager.h"

#include "Social/SocialManager.h"

namespace RKUtils {
	void RKFile_InitModule();
}

MainGame * MainGame::p_Instance = NULL;
MainGame::MainGame()
{
	this->retain();
    PauseTheGame = false;
	p_loadingStep = 0;

	p_BGLoading = nullptr;
	p_loadingText = nullptr;

}


MainGame::~MainGame()
{
	this->autorelease();
}

bool MainGame::init()
{
	p_IsInitialized = false;
	//
	Utility::InitSizeGame();
	//
	//
	this->scheduleUpdate();
	return true;
}
//virtual bool update();
// there's no 'id' in cpp, so we recommend returning the class instance pointer
cocos2d::Scene* MainGame::scene()
{
	// 'scene' is an autorelease object
	cocos2d::Scene *scene = cocos2d::Scene::create();
	// 'layer' is an autorelease object
	MainGame *layer = MainGame::create();
	layer->setName("main_game");
	// add layer as a child to scene
	scene->addChild(layer);
	//
	scene->addChild(Control::InputManager::create());
	// return the scene
	return scene;
}

bool MainGame::InitLoadingScreen()
{
	Vec2 winsize = GetGameSize();
	Vec2 designSize = GetGameDesignSize();
	//

	p_BGLoading = Sprite::create(Utility::File::ResolveNamePath( ((winsize.x / winsize.y > 0.68f) ? "loading_ipad.png" : "loading.png")).GetString());

	p_BGLoading->setScale(winsize.x / designSize.x, winsize.y / designSize.y);
	p_BGLoading->setPosition(winsize.x / 2, winsize.y / 2);
	p_BGLoading->retain();
	//
	p_loadingText = Label::create("Loading...", Utility::File::ResolveNamePath( "fonts/impact.ttf").GetString(), 30.f * winsize.x / designSize.x);
	Vec2 pos_text = Vec2(0, 0);
	if (winsize.x / winsize.y > 0.68f)
	{
		designSize = Vec2(768, 1024);
		pos_text = Vec2(384, 750);
	}
	else
	{
		designSize = Vec2(480, 800);
		pos_text = Vec2(240, 750);
	}

	p_loadingText->setPosition(pos_text.x  * winsize.x / designSize.x, (designSize.y - pos_text.y)* winsize.y / designSize.y);
	p_loadingText->setColor(Color3B::WHITE);
	p_loadingText->retain();

	return true;
}

bool MainGame::InitTheGame(float dt)
{
	if (p_loadingStep == 0)
	{
		RKUtils::RKFile_InitModule();
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 1)
	{
		//load ark file
        InitValue();
		//init screen
		InitLoadingScreen();
		//
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 2)
	{
		File::SaveMgr->LoadTheGameSave();
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 3)
	{
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_TEXTURE_XML);
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 4)
	{
		TextureMgr->Load();
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 5)
	{
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_MATERIAL_DEC_XML);
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_BG_DEC_XML);
		BoostMgr->LoadXMLData();
		p_loadingStep++;
		return false;
	}
	//
	if (p_loadingStep == 6)
	{
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_UI_WIDGET_XML);
		MenuMgr->Init();
		p_loadingStep++;
		return false;
	}
	//load basic object
	if (p_loadingStep == 7)
	{
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_W_OBJECT_DEC_XML);
		p_loadingStep++;
		return false;
	}

	//load sound
	if (p_loadingStep == 8)
	{
        
        XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_EFFECT_DEC_XML);
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_SCENE_3D_DEC_XML);
		MenuMgr->InitSceneManager();
#if USE_CC_AUDIO
		XMLMgr->OnLoadXMLData(Utility::xml::STATE_LOAD_XML::LOAD_SOUND_DEC_XML);
		GetSound->InitSoundEngine();
        GetSound->PlayMusicEffect("MUSIC_MAIN");
#endif
		p_loadingStep++;
		return false;
	}
    
    //init game center
    if (p_loadingStep == 9)
    {
#ifdef GAME_IOS
        m_GCMgr = new Social::GameCenter::GameCenterController();
        m_GCMgr->Init();
#endif
        SocialMgr->Init();
        IAPMgr->OnInitialize();
        p_loadingStep++;
        return false;
    }

	return true;
}


void MainGame::update(float dt)
{
#if CC_REF_LEAK_DETECTION
	Ref::printLeaks();
#endif
    if(PauseTheGame)
        return;

	if (!p_IsInitialized)
	{
		if (XFlatform::IsAtLogoState() == true)
		{
			return;
		}
		//	
		bool res = InitTheGame(dt);
		if (p_loadingText)
		{
			p_loadingText->setString("Loading..." + std::to_string(p_loadingStep * 10) + "%");
		}
		if (!res)
		{
			return;
		}
		p_IsInitialized = true;
		delete p_BGLoading;
		p_BGLoading = NULL;

		delete p_loadingText;
		p_loadingText = NULL;

#ifdef USE_AUDIO_ON_THREAD
		GetSound->OnStartThreadPushSound();
#endif
	}
	else
	{	
		MenuMgr->Update(dt);
	}
}


void MainGame::OnDraw(const Mat4 &transform, uint32_t flags)
{
	auto renderer = Director::getInstance()->getRenderer();
	if (!p_IsInitialized)
	{
		//p_BGLoading->draw(renderer, transform, flags);
		//p_loadingText->draw(renderer, transform, flags);
	}
	else
	{
		//MenuMgr->Draw(renderer, transform, flags);
	}

	renderer = NULL;
}


void MainGame::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
	Node::draw(renderer, transform, flags);
	_customCommand.init(_globalZOrder);
	_customCommand.func = CC_CALLBACK_0(MainGame::OnDraw, this, transform, flags);
	renderer->addCommand(&_customCommand);
	//
	if (!p_IsInitialized)
	{
		if (p_BGLoading)
		{
			p_BGLoading->visit(renderer, transform, flags);
			p_loadingText->visit(renderer, transform, flags);
		}
	}
	else
	{
		MenuMgr->Visit(renderer, transform, flags);
	}
}

void MainGame::EndScence()
{
	this->unscheduleUpdate();
}
