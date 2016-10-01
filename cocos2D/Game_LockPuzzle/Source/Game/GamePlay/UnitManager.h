#ifndef __UINIT_MANAGER_H__
#define __UINIT_MANAGER_H__

#include "ControllableUnit.h"
#include "cocos2d.h"

using namespace cocos2d;
using namespace RKUtils;

//#define USE_RANDOM_LEVEL
//#define RECORRECT_HINT

enum STATE_GAME
{
	STATE_NONE = 0,
	STATE_MOVE,
    STATE_NEXT_STEP,
	STATE_FINISH,
	STATE_FAILED,
	STATE_COUNT
};

struct UIDecVisible
{
	RKString name_ui;
	RKString name_resource;
	bool IsVisible;
public:
	UIDecVisible()
	{
		name_ui = "";
		name_resource = "";
		IsVisible = true;
	}
};

struct DecGameMode
{
	std::vector<UIDecVisible*> ui_dec;
	std::map<RKString,xml::BasicDec *> p_GamePlay_Item_;
	int time_init_each_mode;
public:
	DecGameMode()
	{
		ui_dec.clear();
		p_GamePlay_Item_.clear();
		time_init_each_mode = 0;
	}
	virtual ~DecGameMode()
	{
		ui_dec.clear();
		p_GamePlay_Item_.clear();
	}

	xml::BasicDec * GetGamePlayItemByName(RKString name)
	{
		if (p_GamePlay_Item_.size() > 0 && p_GamePlay_Item_.find(name) != p_GamePlay_Item_.end())
			return p_GamePlay_Item_.at(name);
		return 0;
	}

	int GetTimeInit() { return time_init_each_mode; }
};

struct HLLink : public BasicUnit
{
	Vec2	   IdxObjectContain;
	int		   number_count;
public:
	HLLink()
	{
		IdxObjectContain = Vec2(0, 0);
		number_count = 0;
	}

	virtual ~HLLink()
	{
		IdxObjectContain = Vec2(0, 0);
		number_count = 0;
	}
};


struct GameLevelDec
{
	bool move_diagonally;
	std::vector<std::vector<int>> value;
	std::vector<int> hint;
	std::map<int, int> index_have_lock;

	int number_move_diagonnally;

	int number_start_random;
	Vec2 range_random_number;
    
    bool Is_level_for_creation;

	int getValueAt(int i, int j)
	{
		if ((unsigned int)j >= value.size())
		{
			PASSERT2(false, "out of stack !");
			return 0;
		}

		auto line = value.at(j);

		if ((unsigned int)i >= line.size())
		{
			PASSERT2(false, "out of stack !");
			return 0;
		}
		return line.at(i);
	}

	void SetValueAt(int i, int j, int value_i)
	{
		if ((unsigned int)j >= value.size())
		{
			PASSERT2(false, "out of stack !");
			return ;
		}

		auto line = value.at(j);

		if ((unsigned int)i >= line.size())
		{
			PASSERT2(false, "out of stack !");
			return ;
		}

		line.erase(line.begin() + i);
		line.insert(line.begin() + i, value_i);
	}

	GameLevelDec()
	{
		move_diagonally = false;
		value.clear();
		hint.clear();
		index_have_lock.clear();
		number_move_diagonnally = -2;

		number_start_random = 0;
        
        Is_level_for_creation = false;
	}

	virtual ~GameLevelDec()
	{
		move_diagonally = false;
		value.clear();
		hint.clear();
		index_have_lock.clear();

		number_move_diagonnally = -2;
		number_start_random = 0;
        
        Is_level_for_creation = false;
	}
};

class UnitManager
{
private:
	//for the hint
	BasicUnit * p_hand_hint;
	BasicUnit * p_mini_circle_hint;

	//vari contain the control unit of game
	Vector<ControllableUnit* >	p_ListControllableObject;

	//vari contain line object link unit of game
	Vector<HLLink* >			p_listObjectLinkHL;

	//vari of the text notice of AP
	Vector<BasicUnit* >			p_listFlyTextNotice;

	//list color object highlight as stack
	std::vector<Color4B>		p_color_hight_light;

	//list of index of controlled object under hightlight
	std::vector<int>			p_List_current_idx_link;

	//check the index of object as same as the given result
	int *						p_currentStateIdxLink;

	//nothing
	int							p_current_index_object_controll;

	//size width and height of object ex: 4x4 or 5x5
	Vec2						p_size_list_controllable_object;

	//list of index of controlled object have lock inside
	RKList<int>					p_current_number_object_have_lock;

	//nothing
	std::map<RKString, xml::BasicDec *>		p_gameplay_dec;

	//use to show hint
	bool									p_IsUseHintShow;

	//state move/finish/failed of game
	STATE_GAME								p_current_state_game;

	//use diagonally move
	bool									p_diagonally_move;
	int										p_current_move_diagonally_remain;
	int										p_number_move_dia;

	unsigned int							p_number_failed;

	int InsertIdxLink(int idx);
	bool IsTheIdxNearThePreviousIdx(int idx); // use the 8 square
	void RemoveTheAfterIdxLink(int idx);
	Vec2  CotainIdx(int idx); // Vec2(x,y) x is idx , y number of idx
    
    bool p_IsLevelCreation;
    int  p_count_move_diagonally;

	//for the effect zoom to pause btn
	int p_current_node_have_effect;
	bool is_zoom_effect;

protected:
	//control function

	void PerformLinkObjectTogether();

	HLLink* GenerateObjectLink(ControllableUnit* a, ControllableUnit* b);

	void OnProcessNextLevel(int current_number);

	int InitStepLock(int& chance_each); //return number_lock
	void GenStepLock(int& chance_each , int& number_ins,Vec2 step_range, xml::BasicDec* _basic_dec);

#if defined USE_RANDOM_LEVEL
public:
	static void GenRandomLevel(GameLevelDec * level);
	static Vec2 GetDirectMove(int t);
#else
	Vec2 GetDirectMove(int t);
	void GenRandomLevel(GameLevelDec * level);
#endif

public:
	UnitManager();
	~UnitManager();

	bool InitAllObjectWithParam(GameLevelDec * game_mode_level);

	void DrawAllObject(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void VisitAllObject(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void UpdateAllObject(float dt);

	Vec2 GetSizeListObject() { return p_size_list_controllable_object; }
	void SetSizeListObject(Vec2 val){ p_size_list_controllable_object = val; }

	Vec2 GetNumberCotainOfObjectAtlocation(int x, int y); //x is number contain, y is the index contain

	void SetActionForControlObject(RKString name_action);

	void SetColorHL(Color4B val , int idx) { 
		if (idx >= 0 && p_color_hight_light.size() > 0 && (unsigned int)idx < p_color_hight_light.size())
		{
			p_color_hight_light.erase(p_color_hight_light.begin() + (unsigned int)idx);
			p_color_hight_light.insert(p_color_hight_light.begin() + (unsigned int)idx, val);
		}
	}

	void InsertColorHL(Color4B val)
	{
		p_color_hight_light.push_back(val);
	}

	Color4B GetColorHLAtIdx(int idx)
	{
		if ((unsigned int)idx >= 0 && p_color_hight_light.size() >0 && (unsigned int)idx < p_color_hight_light.size())
		{
			return p_color_hight_light.at((unsigned int)idx);
		}

		return p_color_hight_light.at(0);
	}


	void InsertGamePlayDec(RKString str, xml::BasicDec * dec)
	{
		p_gameplay_dec.insert(std::pair<RKString, xml::BasicDec*>(str, dec));
	}

	xml::BasicDec * GetGamePlayDec(RKString name)
	{
		if (p_gameplay_dec.size() > 0 && p_gameplay_dec.find(name) != p_gameplay_dec.end())
			return p_gameplay_dec.at(name);
		return 0;
	}

	ControllableUnit* GetUnitAtIdx(int idx)
	{
		if (idx < 0 || idx >= p_ListControllableObject.size())
		{
			return 0;
		}
		return p_ListControllableObject.at(idx);
	}

	bool IsUseHintShow() { return p_IsUseHintShow; }
	void IsUseHintShow(bool b) { p_IsUseHintShow = b; }

	void SetObjectFlyTo(RKString resource, Vec2 pos_from, Vec2 pos_to, int number ,float size = 0.f, Color4B color = Color4B::WHITE, float delay_first = 0.f);

	void SetTextFlyTime(int addition_time, Vec2 pos_from, float size = 0.f, Color4B color = Color4B::WHITE, float delay_first = 0.f);

	void SetTextFlyScore(int number, Vec2 pos_from, float size = 0.f, Color4B color = Color4B::WHITE, float delay_first = 0.f);

	void SetObjFlyFromPos(RKString resource , RKString text, Vec2 pos_from , Vec2 pos_to, RKString tag_name,int number, float size = 0.f, Color4B color = Color4B::WHITE, float delay_first = 0.f);

	int IsFinishLevel();

	STATE_GAME GetCurrentGameState() { return p_current_state_game; }
	void ResetCurrentGameState() { p_current_state_game = STATE_GAME::STATE_NONE; }

	void ShowHintWithListIdx(std::vector<int> list);
};


#endif //__UINIT_MANAGER_H__

