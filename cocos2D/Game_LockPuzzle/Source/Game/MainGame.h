#ifndef __MAIN_GAME_H__
#define __MAIN_GAME_H__
#ifdef GAME_IOS
#include "Social/GameCenter/GameCenterController.h"
#endif
#include "DisplayView.h"
#include "cocos2d.h"

using namespace Utility;
class MainGame : public Layer
{
private:
	static MainGame * p_Instance;
	CustomCommand _customCommand;

	bool p_IsInitialized;

	int p_loadingStep;

	Sprite * p_BGLoading;
	Label  * p_loadingText;
    
    bool PauseTheGame;
#ifdef GAME_IOS
    Social::GameCenter::GameCenterController * m_GCMgr;
#endif
public:
	// implement the "static node()" method manually
	CREATE_FUNC(MainGame);
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	//virtual bool update();
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();
	//
	static MainGame * GetInstance()
	{
		if (!p_Instance)
		{
			p_Instance = new MainGame();
		}
		return p_Instance;
	}
	MainGame();
	~MainGame();
	//
	void EndScence();
	//update and main method
	virtual void update(float dt);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags) ;

	void OnDraw(const Mat4&transform, uint32_t flags);
	//
	bool InitTheGame(float dt);

	bool InitLoadingScreen();
	//
	//method with cocos2d engine

	void InitForFirstScence(Director * d)
	{
		d->runWithScene(MainGame::scene());
	}
    
#ifdef GAME_IOS
    Social::GameCenter::GameCenterController * GetGameCenterMgr() { return m_GCMgr;}
#endif
    
    void SetPauseTheGame(bool pause) { PauseTheGame = pause;}

};

#define GameMgr MainGame::GetInstance()

#endif //__MAIN_GAME_H__
