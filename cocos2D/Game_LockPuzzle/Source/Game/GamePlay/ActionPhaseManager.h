#ifndef __ACTION_PHASE_MANAGER__
#define __ACTION_PHASE_MANAGER__

#include "UnitManager.h"
#include "cocos2d.h"
#include "LoadDataXML.h"
using namespace cocos2d;
using namespace RKUtils;


class APManager : Layer
{
private:
	UnitManager * p_unitManager;

	std::vector<GameLevelDec *> p_levelGame;

	float p_time_delay_next_level;
	int	  p_level_play;

	xml::BasicDec * p_value_xml;

	void LoadAPParam(TiXmlDocument * p);


	void LoadLevelParam(TiXmlDocument * p);

#if defined USE_RANDOM_LEVEL || defined RECORRECT_HINT
public:
	static GameLevelDec * CreateLevelDec(TiXmlElement * level_dec);
#else
	GameLevelDec * CreateLevelDec(TiXmlElement * level_dec);
#endif

public:
	APManager();
	~APManager();

	bool Init(int level = 0);
	virtual void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	virtual void visit(Renderer *renderer, const Mat4& transform, uint32_t flags);

	void Update(float dt);

	bool InitNewTable(int level);


	void LoadDataAPFromXML();
	void LoadDataLevelFromXML();

	void ShowHint();

	bool IsShowHint() { return p_unitManager->IsUseHintShow(); }
	void IsShowHint(bool val) { p_unitManager->IsUseHintShow(val); }

	UnitManager * GetUnitMgr() { return p_unitManager; }

	GameLevelDec * GetLevelAtIdx(int idx);
	
	GameLevelDec * GetCurrentLevel() { return GetLevelAtIdx(p_level_play); }

	int GetCurrentLevelNumber() { return p_level_play; }

	void SetDelayTime();

};


#endif //__ACTION_PHASE_MANAGER__

