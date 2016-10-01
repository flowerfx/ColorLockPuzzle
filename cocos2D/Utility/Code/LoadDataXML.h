#ifndef LOAD_DATA_XML_H
#define LOAD_DATA_XML_H

#define USE_TINY_XML2
#include "math/CCGeometry.h"
#include "base/CCVector.h"
#ifdef USE_TINY_XML2
#include "../../external/tinyxml2/tinyxml2.h"
#else
#include "../../external/tinyxml/tinyxml.h"
#endif
#include "FileManager.h"
#include "DisplayView.h"
#include "RKString_Code/RKString.h"
#ifdef USE_TINY_XML2

#define TiXmlDocument	tinyxml2::XMLDocument
#define TiXmlNode		tinyxml2::XMLNode
#define TiXmlElement	tinyxml2::XMLElement
#endif
using namespace RKUtils;
USING_NS_CC;
namespace Utility
{
	enum UI_STATE_FADE_ACTION
	{
		FADE_MOVE = 1,
		FADE_TRANS = FADE_MOVE << 1,
		FADE_ROTATE = FADE_TRANS << 1,
		FADE_ZOOM = FADE_ROTATE << 1,
		FADE_DELAY = FADE_ZOOM<<1,
		FADE_BENZIER = FADE_DELAY <<1,
		FADE_COUNT = 6
	};

	namespace xml
	{
		enum HARDWARD_PROFILE
		{
			HARDWARE_LOW = 0,
			HARDWARE_MED,
			HARDWARE_HIGH
		};

		enum STATE_LOAD_XML
		{
			LOAD_TEXTURE_XML = 0,
			LOAD_UI_DEC_XML,
			LOAD_BG_DEC_XML,
			LOAD_BASIC_DEC_XML,
			LOAD_MATERIAL_DEC_XML,
			LOAD_SOUND_DEC_XML,
			LOAD_EFFECT_DEC_XML,
			LOAD_UI_WIDGET_XML,
			LOAD_W_OBJECT_DEC_XML,
			LOAD_SCENE_3D_DEC_XML,
			LOAD_TEXTURE_PACKER_DEC_XML,
			STATE_COUNT
		};

		enum TYPE_UI_WIDGET
		{
			WIDGET_NONE = 0,
			WIDGET_BUTTON ,
			WIDGET_PANEL,
			WIDGET_FONT,
			WIDGET_CHECK_BOX,
			WIDGET_LOADING_BAR,
			WIDGET_TEXT_FIELD,
			WIDGET_SLIDER,
			WIDGET_LIST_VIEW,
			WIDGET_SCROLL_VIEW,
			WIDGET_PAGE_VIEW,
			WIDGET_LAYER,
			WIDGET_LAYOUT,
			WIDGET_RADIO_BTN,
			WIDGET_COUNT
		};

		/*

			use for load atlas texture

			*/

		struct AtlasChild
		{
			Vec2  Size;
			Vec2  Position;
			RKString nameSprite;
		public:
			AtlasChild() :
				Size(0, 0),
				Position(-1, -1)
			{
			}
			virtual ~AtlasChild()
			{
				Size = Vec2(0, 0);
				Position = Vec2(0, 0);
			}
			bool IsValidPosition()
			{
				if (Position.x < 0 || Position.y < 0)
					return false;
				return true;
			}
		};

		struct AtlasTexture
		{
			std::map<RKString, AtlasChild > dDataSize;
			RKString NameImage;
			RKString NamePath;
		public:
			AtlasTexture() :
				NameImage(""),
				NamePath("")
			{
			}
			virtual ~AtlasTexture()
			{
				dDataSize.clear();
			}

			AtlasChild GetAtlasChild(RKString str)
			{
				if (dDataSize.size() > 0 && dDataSize.find(str) != dDataSize.end())
				{
					return dDataSize.at(str);
				}
				AtlasChild a;
				return a;
			}
			void InsertAtlasChild(RKString str, AtlasChild d)
			{
				dDataSize.insert(std::pair<RKString, AtlasChild >(str, d));
			}

			int GetNumberAtlas()
			{
				return (int)dDataSize.size();
			}
		};
        
        struct TextPackerDetail
        {
            RKString texture_path;
            RKString texture_plist;
        public:
            TextPackerDetail()
            {
                texture_path = "";
                texture_plist= "";
            }
            TextPackerDetail(RKString path, RKString plist)
            {
                texture_path = path;
                texture_plist= plist;
            }

			bool IsValid() {
				return texture_path != "";
			}
        };
        
        struct TextPacker
        {
            std::map<RKString , std::vector<TextPackerDetail>> p_list_texture_packer;
            Vec2 SizeFit;
            
        public:
            TextPacker()
            {
            }
            
            virtual ~TextPacker()
            {
                p_list_texture_packer.clear();
            }
            
            TextPackerDetail GetTexPackByName(RKString str , int idx = 0)
            {
                if (p_list_texture_packer.size() > 0 && p_list_texture_packer.find(str) != p_list_texture_packer.end())
                {
					if (idx < 0 || idx >= p_list_texture_packer.at(str).size())
					{
						TextPackerDetail a;
						return a;
					}
                    return p_list_texture_packer.at(str).at(idx);
                }
                TextPackerDetail a;
                return a;
            }
            void InsertTexPack(RKString str, std::vector<TextPackerDetail> d)
            {
                p_list_texture_packer.insert(std::pair<RKString, std::vector<TextPackerDetail> >(str, d));
            }
            
			int GetNumberTextureByName(RKString name)
			{
				if (p_list_texture_packer.size() > 0 && p_list_texture_packer.find(name) != p_list_texture_packer.end())
				{
					return p_list_texture_packer.at(name).size();
				}
				return 0;
			}

            int GetNumberTexPack()
            {
                return (int)p_list_texture_packer.size();
            }
            
        };
        
		/*

		use for load font

		*/

		struct FontDec
		{
			int idx;

			float d_size;
			float d_width_border;
            RKUtils::BYTE d_color[4];
            RKUtils::BYTE d_color_border[4];

			RKString path;
			RKString name;
			bool IsUnicode;

		public:
			FontDec()
			{
				idx = 0;
				path = "";
				name = "";
				d_size = 0;
				d_width_border = 0;
				d_color[0] = 255; d_color[1] = 255; d_color[2] = 255; d_color[3] = 255;
				d_color_border[0] = 255; d_color_border[1] = 255; d_color_border[2] = 255; d_color_border[3] = 255;
				IsUnicode = false;
			}
			virtual ~FontDec()
			{

			}

		};


		/*

		use for load abstract basic object

		*/
		
		struct AnimatureDec
		{
			RKString name;
			RKString pathTexture;
			RKString pathPlist;
			RKString PathJson;

			float SpeedScale;
			int idxPlay;
		public:
			AnimatureDec()
			{
				name = "";
				pathTexture = "";
				pathPlist = "";
				PathJson = "";
				SpeedScale = 1.0f;
				idxPlay = 0;
			}
			virtual ~AnimatureDec()
			{
				//TODO:
			}
		};

		struct AnimateDec
		{
			RKList<RKString> p_SpriteFrame;
			float time_delay;
			int loop;
			RKString source_sprite;
			int tag_action;
		public:
			AnimateDec()
			{
				time_delay = 0.1f;
				loop = -1; //loop forever

				source_sprite = "";
				tag_action = 0;
			}
			virtual ~AnimateDec()
			{
				p_SpriteFrame.Clear();
			}
			void InsertSpriteFrameName(RKString name)
			{
				p_SpriteFrame.Insert(name, p_SpriteFrame.Size());
			}
			RKString GetSpriteFrameNameAtIdx(int idx)
			{
				if (idx > 0 && (unsigned int)idx < p_SpriteFrame.Size())
				{
					return p_SpriteFrame.GetAt((unsigned int)idx);
				}
				return "";
			}
			int GetNumberSprite()
			{
				return p_SpriteFrame.Size();
			}

		};

		struct BasicDec
		{
		private:
			std::map<RKString, Color4B>	__name_color_;

			std::map<RKString, Vec2>	__name_vector_;

			std::map<RKString, std::string>	__name_string_;

			std::map<RKString, float>	__name_float_;

			std::map<RKString, int>		__name_int_;
		public:
			BasicDec()
			{

			}
			virtual ~BasicDec()
			{
				__name_color_.clear();
				__name_vector_.clear();
				__name_float_.clear();
				__name_int_.clear();
				__name_string_.clear();
			}
			//insert
			void InsertDataInt(RKString name, int data)
			{
				__name_int_.insert(std::pair<RKString, int>(name, data));
			}
			void InsertDatafloat(RKString name, float data)
			{
				__name_float_.insert(std::pair<RKString, float>(name, data));
			}
			void InsertDataChar(RKString name, const char * data)
			{
				__name_string_.insert(std::pair<RKString,std::string>(name, data));
			}
			void InsertDataVector2(RKString name, Vec2 data)
			{
				__name_vector_.insert(std::pair<RKString, Vec2>(name, data));
			}
			void InsertDataColor(RKString name, Color4B data)
			{
				__name_color_.insert(std::pair<RKString, Color4B>(name, data));
			}
			//check find data
			bool HaveDataInt(RKString name)
			{
				if (__name_int_.size() > 0 && __name_int_.find(name) != __name_int_.end())
					return true;
				return false;
			}
			bool HaveDatafloat(RKString name)
			{
				if (__name_float_.size() > 0 && __name_float_.find(name) != __name_float_.end())
					return true;
				return false;
			}
			bool HaveDataChar(RKString name)
			{
				if (__name_string_.size() > 0 && __name_string_.find(name) != __name_string_.end())
					return true;
				return false;
			}
			bool HaveDataVector2(RKString name)
			{
				if (__name_vector_.size() > 0 && __name_vector_.find(name) != __name_vector_.end())
					return true;
				return false;
			}
			bool HaveDataColor(RKString name)
			{
				if (__name_color_.size() > 0 && __name_color_.find(name) != __name_color_.end())
					return true;
				return false;
			}

			//get data
			int GetDataInt(RKString name)
			{
				if (__name_int_.size() > 0 && __name_int_.find(name) != __name_int_.end())
					return __name_int_.at(name);
				return 0;
			}
			float GetDatafloat(RKString name)
			{
				if (__name_float_.size() > 0 && __name_float_.find(name) != __name_float_.end())
					return __name_float_.at(name);
				return 0;
			}
			const char * GetDataChar(RKString name)
			{
				if (__name_string_.size() > 0 && __name_string_.find(name) != __name_string_.end())
					return __name_string_.at(name).c_str();
				return "";
			}
			Vec2 GetDataVector2(RKString name)
			{
				if (__name_vector_.size() > 0 && __name_vector_.find(name) != __name_vector_.end())
					return __name_vector_.at(name);
				return Vec2(0, 0);
			}
			Color4B GetDataColor(RKString name)
			{
				if (__name_color_.size() > 0 && __name_color_.find(name) != __name_color_.end())
					return __name_color_.at(name);
				return Color4B::WHITE;
			}

			//replace 
			void ReplaceDataInt(RKString name, int data)
			{
				if (__name_int_.find(name) != __name_int_.end())
				{
					__name_int_.erase(name);
				}
				__name_int_.insert(std::pair<RKString, int>(name, data));
			}
			void ReplaceDatafloat(RKString name, float data)
			{
				if (__name_float_.find(name) != __name_float_.end())
				{
					__name_float_.erase(name);
				}
				__name_float_.insert(std::pair<RKString, float>(name, data));
			}
			void ReplaceDataChar(RKString name, const char * data)
			{
				if (__name_string_.find(name) != __name_string_.end())
				{
					__name_string_.erase(name);
				}
				__name_string_.insert(std::pair<RKString, std::string>(name, data));
			}
			void ReplaceDataVector2(RKString name, Vec2 data)
			{ 
				if (__name_vector_.find(name) != __name_vector_.end())
				{
					__name_vector_.erase(name);
				}
				__name_vector_.insert(std::pair<RKString, Vec2>(name, data));
			}
			void ReplaceDataColor(RKString name, Color4B data)
			{
				if (__name_color_.find(name) != __name_color_.end())
				{
					__name_color_.erase(name);
				}
				__name_color_.insert(std::pair<RKString, Color4B>(name, data));
			}
		};

		struct ObjectDec
		{
			int idx;
			RKString name;
			//
			char *			name_source;
			AnimatureDec *	animature_dec;
			AnimateDec *	animation_dec;
			FontDec *		text_dec;
			//
			BasicDec * Move_delay_dec;
			//
			BasicDec * Position_dec;
			//
			BasicDec * Size_dec;
			//
			BasicDec * Opacity_dec;
			//
			BasicDec * Rotate_dec;
			//
			BasicDec * Rotate_around_dec;
			//
			BasicDec * Color_dec;
			//
			BasicDec * Process_dec;
			//
			int		   StickScreen;
		public:
			ObjectDec()
			{
				idx = 0;
				name = "";
				name_source			= NULL;
				animature_dec		= NULL;
				animation_dec		= NULL;
				Move_delay_dec		= NULL;
				Position_dec		= NULL;
				Size_dec			= NULL;
				Opacity_dec			= NULL;
				Rotate_dec			= NULL;
				Rotate_around_dec	= NULL;
				Color_dec			= NULL;
				Process_dec			= NULL;
				text_dec			= NULL;
				StickScreen = 0;
			}
			virtual ~ObjectDec()
			{
				if (name_source){
					delete name_source;
					name_source = NULL;
				}
				if (animature_dec){
					delete animature_dec;
					animature_dec = NULL;
				}
				if (Position_dec){
					delete Position_dec;
					Position_dec = NULL;
				}
				if (Size_dec){
					delete Size_dec;
					Size_dec = NULL;
				}
				if (Opacity_dec){
					delete Opacity_dec;
					Opacity_dec = NULL;
				}
				if (Rotate_dec){
					delete Rotate_dec;
					Rotate_dec = NULL;
				}
				if (Rotate_around_dec){
					delete Rotate_around_dec;
					Rotate_around_dec = NULL;
				}
				if (Color_dec){
					delete Color_dec;
					Color_dec = NULL;
				}
				if (Move_delay_dec)
				{
					delete Move_delay_dec;
					Move_delay_dec = NULL;
				}
				if (Process_dec)
				{
					delete Process_dec;
					Process_dec = NULL;
				}

				if (text_dec)
				{
					delete text_dec;
					text_dec = NULL;
				}
			}

		};

		struct Wide_ObjectDec
		{
			RKString name_object;

			int type_object;
			//
			RKString		name_source;
			AnimatureDec *	animature_dec;
			AnimateDec *	animation_dec;
			FontDec *		text_dec;
			//
			BasicDec * OriginValue;
			BasicDec * FirstValue;
			BasicDec * CommonValue;
			//
			std::map<RKString, RKString> p_list_command;
			//
			Wide_ObjectDec *			 parent_object;
			std::vector<Wide_ObjectDec*> list_child_object;

			Vec2 design_size;

		public:
			Wide_ObjectDec()
			{
				name_object = "";
				type_object = -1;
				//
				name_source = "";
				animature_dec = nullptr;
				animation_dec = nullptr;
				text_dec = nullptr;
				//
				OriginValue = new BasicDec();
				CommonValue = new BasicDec();
				FirstValue = new BasicDec();
				//
				OriginValue->InsertDataVector2("pos_ratio", Vec2(0, 0));
				OriginValue->InsertDataVector2("scale_ratio", Vec2(1, 1));
				OriginValue->InsertDatafloat("rotate", 0.f);
				OriginValue->InsertDataInt("opacity", 255);
				OriginValue->InsertDataColor("tint", Color4B::WHITE);
				OriginValue->InsertDataInt("Process", 100);
				//
				FirstValue->InsertDataVector2("pos_ratio", Vec2(0, 0));
				FirstValue->InsertDataVector2("scale_ratio", Vec2(1, 1));
				FirstValue->InsertDatafloat("rotate", 0.f);
				FirstValue->InsertDataInt("opacity", 255);
				FirstValue->InsertDataColor("tint", Color4B::WHITE);
				FirstValue->InsertDataInt("Process", 100);
				//
				design_size = Vec2(0, 0);
				//
				CommonValue->InsertDataInt("visible", 1);

				parent_object = nullptr;//not delete the parent;
				list_child_object.clear();
				p_list_command.clear();
			}

			virtual ~Wide_ObjectDec()
			{
				CC_SAFE_DELETE(OriginValue);
				CC_SAFE_DELETE(CommonValue);
				CC_SAFE_DELETE(FirstValue);

				type_object = -1;
				name_source.Clear();
				CC_SAFE_DELETE(animature_dec);
				CC_SAFE_DELETE(animation_dec);
				CC_SAFE_DELETE(text_dec);

				parent_object = nullptr;
				list_child_object.clear();
				p_list_command.clear();
			}

			void InsertWideObjectChild(Wide_ObjectDec * child_object)
			{
				list_child_object.push_back(child_object);
			}

			const char* GetCommandByName(RKString name) const
			{
				if (p_list_command.size() > 0 && p_list_command.find(name) != p_list_command.end())
				{
					return p_list_command.at(name).GetString();
				}
				return nullptr;
			}

			void InsertCommand(RKString name, RKString command)
			{
				p_list_command.insert(std::pair<RKString, RKString>(name, command));
			}

			unsigned int GetCommandSize() {
				return p_list_command.size();
			}

			std::pair<RKString, RKString> GetIterCommandAtIdx(int idx)
			{
				int i = 0;
				for (auto it = p_list_command.begin(); it != p_list_command.end(); it++)
				{
					if (i == idx)
					{
						return std::pair<RKString, RKString>(it->first, it->second);
					}
					i++;
				}
				return std::pair<RKString, RKString>("", "");
			}
		};

		/*

		use for load UI

		*/

		struct sound_click_dec
		{
			RKString id_click;
			float delayTime;
			int loop;
		public:
			sound_click_dec()
			{
				id_click = "";
				delayTime = 0;
				loop = 0;
			}
		};

		struct ResUIElement
		{
			RKString source;
			RKString type;
			//
			Vec2 origin_PosRatio;
			Vec2 first_PosRatio;
			bool not_fade_pos;
			//
			Vec2 origin_ScaleRatio;
			Vec2 first_ScaleRatio;
			bool not_fade_size;
			//
			float origin_Rotate;
			float first_Rotate;
			bool not_fade_rotate;
			//
			float origin_opacity;
			float first_opacity;
			bool not_fade_opacity;
			//
			int type_press_button;
			float scale_press_down;
			RKString source_hl;
			RKString source_disable;
			//
			int		Tint[3];
			int		TintBorder[4];
			float   BorderWidth;
			int		fontIdx;
			int		align;
			//
			RKList<UI_STATE_FADE_ACTION> actionFade;
			float TimeProcessFade;
			//
			int index;
			int visible;
			int index_appear;

			int stick_screen;

			sound_click_dec * sound_click;
		public:
			ResUIElement()
			{
				source = "";
				type = "";
				//
				origin_PosRatio = Vec2(0, 0);	first_PosRatio = Vec2(0, 0);
				origin_ScaleRatio = Vec2(1, 1); first_ScaleRatio = Vec2(1, 1);
				origin_Rotate = 0;				first_Rotate = 0;
				origin_opacity = 255;			first_opacity = 255;
				//
				type_press_button = 0;
				scale_press_down = 0.75f;
				source_hl = "";
				source_disable = "";
				//
				Tint[0] = 255; Tint[1] = 255; Tint[2] = 255; Tint[3] = 255;
				TintBorder[0] = 255; TintBorder[1] = 255; TintBorder[2] = 255; TintBorder[3] = 255;
				//
				BorderWidth = 0;
				fontIdx = 0;
				align = 0; // 0 is left , 1 is mid , 2 is right
				//
				TimeProcessFade = 0;
				index = 0;
				//
				visible = 1; //mean appear at first
				index_appear = 0; //if index_appear > 0 the first appear will not process it
				stick_screen = 0;
				//
				sound_click = NULL;
				//
				not_fade_pos = false;
				not_fade_size = false;
				not_fade_rotate = false;
				not_fade_opacity = false;
			}
			virtual ~ResUIElement()
			{
				actionFade.Clear();
				CC_SAFE_DELETE(sound_click);
			}

			void SetActionFade(UI_STATE_FADE_ACTION af)
			{
				actionFade.Insert(af, actionFade.Size());
			}

		};

		struct time_process_ui
		{
			float time_process;
			RKString ID_SOUND_FADE_IN;
			RKString ID_SOUND_FADE_OUT;
			float delayTime;
			int loop;
			time_process_ui()
			{
				time_process = 0;
				ID_SOUND_FADE_IN = "";
				ID_SOUND_FADE_OUT = "";
				delayTime = 0;
				loop = 0;
			}
		};

		struct UIMenuDec
		{
		
			RKString name;
			Vec2 DesignSize;
			int max_idx_first;
			RKList<time_process_ui> time_process;
			
			RKList<RKString> p_listChildElementByName;
			std::map<RKString, ResUIElement * > p_listElementChild;		
		public:
			UIMenuDec()
			{
				name = "";
				DesignSize = Vec2(0, 0);
				max_idx_first = 0;
			}
			UIMenuDec(RKString Name, Vec2 size)
			{
				name = Name;
				DesignSize = size;
				max_idx_first = 0;
			}
			virtual ~UIMenuDec()
			{
				p_listElementChild.clear();
				time_process.Clear();
			}
			ResUIElement * GetElementChildByName(RKString str) {
				//
				if (p_listElementChild.size() <= 0 )
				{
					return 0;
				}
				for (auto it = p_listElementChild.begin(); it != p_listElementChild.end(); it++)
				{
					int idx = it->second->index;
					auto strName = it->first.Split("#");
					if (idx == atoi(strName[0].GetString()) && str == strName[1])
					{
						return it->second;
					}
				}
				return 0;
			}
			void InsertChildElement(RKString str, int idx, ResUIElement* e)
			{
				str = RKString(std::to_string(idx)) + "#" + str;
				p_listElementChild.insert(std::pair<RKString, ResUIElement*>(str, e));
			}
			RKList<RKString> GetListChildName(){
				return p_listChildElementByName;
			}
			void InsertNameToList(RKString name)
			{
				p_listChildElementByName.Insert(name, p_listChildElementByName.Size());
			}

			time_process_ui GetTimeProcess(int idx)
			{
				if (idx >= 0 && (unsigned int)idx < time_process.Size())
					return time_process.GetAt((unsigned int)idx);
				time_process_ui temp;
				return temp;
			}

			int GetNumberTimeProcess()
			{
				return time_process.Size();
			}


		};
		

		/*

		use for load UI Advance

		*/
		struct UILayerWidgetDec;
		struct UIMenuWidgetDec;

		struct UIWidgetDec
		{
			RKString NameWidget;

			TYPE_UI_WIDGET typeValue;

			BasicDec * OriginValue;
			BasicDec * FirstValue;

			BasicDec * GeneralValue;

			RKList<UI_STATE_FADE_ACTION> actionFade;
			float TimeProcessFade;
			//
			int index;
			int visible;
			int index_appear;

			sound_click_dec * sound_click;

			UILayerWidgetDec * p_parent_layer;
			UIMenuWidgetDec * p_menu_widget;

			std::map<RKString, RKString> p_list_command;
		public:
			UIWidgetDec()
			{
				NameWidget = "";
				typeValue = WIDGET_NONE;

				OriginValue = new BasicDec();
				FirstValue = new BasicDec();
				GeneralValue = new BasicDec();
				//
				OriginValue->InsertDataVector2("pos_ratio", Vec2(0, 0));
				OriginValue->InsertDataVector2("scale_ratio", Vec2(1, 1));
				OriginValue->InsertDatafloat("rotate", 0.f);
				OriginValue->InsertDataInt("opacity", 255);

				FirstValue->InsertDataVector2("pos_ratio", Vec2(0, 0));
				FirstValue->InsertDataVector2("scale_ratio", Vec2(1, 1));
				FirstValue->InsertDatafloat("rotate", 0.f);
				FirstValue->InsertDataInt("opacity", 255);

				//
				GeneralValue->InsertDataInt("not_fade_pos", 0);
				GeneralValue->InsertDataInt("not_fade_scale", 0);
				GeneralValue->InsertDataInt("not_fade_rotate", 0);
				GeneralValue->InsertDataInt("not_fade_opacity", 0);
				GeneralValue->InsertDataColor("tint", Color4B::WHITE);
				//
				sound_click = nullptr;
				p_menu_widget = nullptr;

				TimeProcessFade = 1.0f;
				index_appear = 0;
				visible = 1;
				index = 0;

				p_parent_layer = nullptr;//not delete the parent;
			}

			virtual ~UIWidgetDec()
			{
				CC_SAFE_DELETE(sound_click);
				CC_SAFE_DELETE(OriginValue);
				CC_SAFE_DELETE(FirstValue);
				CC_SAFE_DELETE(GeneralValue);

				actionFade.Clear();
				p_parent_layer = nullptr;
				p_menu_widget = nullptr;
			}

			void SetActionFade(UI_STATE_FADE_ACTION af)
			{
				actionFade.Insert(af, actionFade.Size());
			}

			const char* GetCommandByName(RKString name) const
			{
				if (p_list_command.size() > 0 && p_list_command.find(name) != p_list_command.end())
				{
					return p_list_command.at(name).GetString();
				}
				return nullptr;
			}

			void InsertCommand(RKString name, RKString command)
			{
				p_list_command.insert(std::pair<RKString, RKString>(name, command));
			}

			unsigned int GetCommandSize() {
				return p_list_command.size();
			}

			std::pair<RKString, RKString> GetIterCommandAtIdx(int idx)
			{
				int i = 0;
				for (auto it = p_list_command.begin(); it != p_list_command.end(); it++)
				{
					if (i == idx)
					{
						return std::pair<RKString, RKString>(it->first, it->second);
					}
					i++;
				}
				return std::pair<RKString, RKString>("", "");
			}

		};

		struct UILayerWidgetDec : public UIWidgetDec
		{
			std::vector<UILayerWidgetDec *> p_list_child_widget;

		public:
			UILayerWidgetDec()
			{
				p_list_child_widget.clear();
			}

			virtual ~UILayerWidgetDec()
			{

				for (unsigned int i = 0; i < p_list_child_widget.size(); i++)
				{
					CC_SAFE_DELETE(p_list_child_widget.at((unsigned int)i));
				}
				p_list_child_widget.clear();
			}

			void InsertChildWidget(UILayerWidgetDec * widget)
			{
				p_list_child_widget.push_back(widget);
			}

			UILayerWidgetDec * GetChildWidgetByName(RKString name)
			{
				for (unsigned int i = 0; i < p_list_child_widget.size(); i++)
				{
					if (p_list_child_widget.at(i)->NameWidget == name)
					{
						return p_list_child_widget.at(i);
					}
				}
				return 0;
			}

			bool IsTopLayerMenu()
			{
				return (p_parent_layer == nullptr);
			}

		};

		struct UIMenuWidgetDec
		{
			RKString NameMenuWidget;
			Vec2 DesignSize;
			int max_idx_first;

			RKList<time_process_ui>					time_process;
			RKList<RKString>						p_listChildElementByName;
			std::map<RKString, UILayerWidgetDec*>	p_listElementChild;
		public:
			UIMenuWidgetDec()
			{
				NameMenuWidget = "";
				DesignSize = Vec2(0, 0);
				max_idx_first = 0;
			}
			UIMenuWidgetDec(RKString Name, Vec2 size)
			{
				NameMenuWidget = Name;
				DesignSize = size;
				max_idx_first = 0;
			}
			virtual ~UIMenuWidgetDec()
			{
				p_listElementChild.clear();
				time_process.Clear();
			}
			UILayerWidgetDec * GetElementChildByName(RKString str) {
				//
				if (p_listElementChild.size() <= 0)
				{
					return 0;
				}
				for (auto it = p_listElementChild.begin(); it != p_listElementChild.end(); it++)
				{
					int idx = it->second->index;
					auto strName = it->first.Split("#");
					if (idx == atoi(strName[0].GetString()) && str == strName[1])
					{
						return it->second;
					}
				}
				return 0;
			}
			void InsertChildElement(RKString str, int idx, UILayerWidgetDec* e)
			{
				str = RKString(std::to_string(idx)) + "#" + str;
				p_listElementChild.insert(std::pair<RKString, UILayerWidgetDec*>(str, e));
			}

			RKList<RKString> GetListChildName()
			{
				return p_listChildElementByName;
			}

			void InsertNameToList(RKString name)
			{
				p_listChildElementByName.Insert(name, p_listChildElementByName.Size());
			}

			time_process_ui GetTimeProcess(int idx)
			{
				if (idx >= 0 && (unsigned int)idx < time_process.Size())
					return time_process.GetAt((unsigned int)idx);
				time_process_ui temp;
				return temp;
			}

			int GetNumberTimeProcess()
			{
				return time_process.Size();
			}
		};

		/*

			use for load background data

		*/

		struct LayerData
		{
			RKString p_NameLayer;
			int		p_index_layer;
			int     p_loopAction;


			Vec2  p_DistanceEachObject;
			Vec2  p_delta_distance_x;
			Vec2  p_delta_distance_y;
			//
			float p_TimeDelay;
			float p_processTime;
			Vec2  p_number_object;
			//

			bool  p_IsObjectPosRandom; // the pos each object in layer random in screen not squence together
			bool  p_IsScaleRandom;
			bool  p_IsRotateRandom;
			bool  p_IsOpacityRandom;
			bool  p_IsColorRandom;
			bool  p_IsProcessRandom;

			bool  p_isObjectRandom;

			int  p_idxAppearRandomOnEdgeBound;
			Vec2  p_TimeMoveRandom;
            cocos2d::Rect  p_BoundRandomPos;
			Vec2  p_randomScale;
			Vec2  p_randomRotate;
			Vec2  p_randomOpacity;
			Vec2  p_randomColor_r;
			Vec2  p_randomColor_g;
			Vec2  p_randomColor_b;
			Vec2  p_timeDelayRandom;

			Vec2  p_timeProcessRandom;
			Vec2  p_percentProcessRandom;

			Vec2  p_direct_move_random_x;
			Vec2  p_direct_move_random_y;

			std::map<RKString, ObjectDec *> p_listObject;

			std::map<RKString, BasicDec *>   p_list_layer_element_dec;
		public:

			LayerData()
			{
				p_DistanceEachObject = Vec2(0, 0);
				p_delta_distance_x = Vec2(0, 0);
				p_delta_distance_y = Vec2(0, 0);
				p_number_object = Vec2(1, 2); //range object init at 1 to 2
				p_TimeDelay = 0.f;
				p_processTime = 0.25f;

				p_TimeMoveRandom = Vec2(p_processTime, p_processTime);
				p_timeDelayRandom = Vec2(0, 0);

				p_idxAppearRandomOnEdgeBound = 0;
				p_IsObjectPosRandom = false;
				p_IsScaleRandom = false;
				p_IsRotateRandom = false;
				p_IsOpacityRandom = false;
				p_IsColorRandom = false;
				p_IsProcessRandom = false;

				p_isObjectRandom = true;

				p_randomScale = Vec2(100, 100); // scale 1
				p_randomRotate = Vec2(0, 0);/// angle 0
				p_randomOpacity = Vec2(255, 255); //opacity 255
				p_randomColor_r = p_randomColor_g = p_randomColor_b = Vec2(255, 255); //white color

				p_timeProcessRandom = Vec2(0, 0);
				p_percentProcessRandom = Vec2(100, 100);

				p_BoundRandomPos = cocos2d::Rect(0, 0, GetGameSize().width, GetGameSize().height);

				p_direct_move_random_x = Vec2(0, 0);
				p_direct_move_random_y = Vec2(0, 0);

				p_loopAction = -1;
			}
			virtual ~LayerData()
			{
				p_listObject.clear();
				p_list_layer_element_dec.clear();
			}

			ObjectDec * GetObjectBGDecByName(RKString name)
			{
				if (p_listObject.size() > 0 && p_listObject.find(name) != p_listObject.end())
				{
					return p_listObject.at(name);
				}
				return NULL;
			}

			ObjectDec * GetObjectBGDecByIdx(int idx)
			{
				if ((int)p_listObject.size() < 0 || (unsigned int)idx >= p_listObject.size())
					return NULL;
				for (auto it = p_listObject.begin(); it != p_listObject.end(); ++it)
				{
					if (idx <= 0)
						return it->second;
					else
						idx--;
				}
				return nullptr;
			}

			void InsertObjectBGDec(RKString name, ObjectDec * val)
			{
				p_listObject.insert(std::pair<RKString, ObjectDec*>(name, val));
			}

			int GetSizeObjDec() {
				return p_listObject.size();
			}

			float GetRandomObject(Vec2 range, int div = 0)
			{
				float val = 0;
				int random = GetRandom((int)range.x, (int)range.y);
				if (range.x == range.y || range.x == 0)
				{
					if (div != 0)
					{
						return (float)random / div;
					}
					else
					{
						return (float)random;
					}
				}
				if (div != 0)
				{
					val = (float)random / div;
				}
				else
				{
					val = (float)random;
				}
				return val;
			}

			int GetNumberObject()
			{
				int val = (int)GetRandomObject(p_number_object);
				return val;
			}

			int GetDistanceObject()
			{
				int val = (int)GetRandomObject(p_DistanceEachObject);
				return val;
			}

			float direct_x_Obj()
			{
				float val = GetRandomObject(p_delta_distance_x, MAX((int)p_delta_distance_x.x, (int)p_delta_distance_x.y));
				return val;
			}

			float direct_y_Obj()
			{
				float val = GetRandomObject(p_delta_distance_y, MAX((int)p_delta_distance_y.x, (int)p_delta_distance_y.y));
				return val;
			}

			float GetScaleRandom()
			{
				float val = GetRandomObject(p_randomScale, 100);
				return val;
			}

			float GetRotateRandom()
			{
				float val = GetRandomObject(p_randomScale);
				return val;
			}

            RKUtils::BYTE GetOpacityRandom()
			{
				RKUtils::BYTE val = (RKUtils::BYTE)GetRandomObject(p_randomOpacity);
				return val;
			}

			Color4B GetColorRandom()
			{
				RKUtils::BYTE val_r = (RKUtils::BYTE)GetRandomObject(p_randomColor_r);
				RKUtils::BYTE val_g = (RKUtils::BYTE)GetRandomObject(p_randomColor_g);
				RKUtils::BYTE val_b = (RKUtils::BYTE)GetRandomObject(p_randomColor_b);
				Color4B val = Color4B(val_r, val_g, val_b, 255);
				return val;
			}

            cocos2d::Rect GetBoundRandomPos()
			{
				return
					cocos2d::Rect(
					Vec2(p_BoundRandomPos.origin.x * GetGameSize().width, p_BoundRandomPos.origin.y * GetGameSize().height),
					cocos2d::Size(p_BoundRandomPos.size.width * GetGameSize().width, p_BoundRandomPos.size.height * GetGameSize().height));
			}

			float GetTimeMoveRandom()
			{
				float val = GetRandomObject(p_TimeMoveRandom * 10);
				return val / 10.f;
			}

			float GetDelayTimeRandom()
			{
				float val = GetRandomObject(p_timeDelayRandom * 10);
				return val / 10.f;
			}

			Vec2 GetVectorMoveRandom()
			{
				Vec2 val;
				val.x = GetRandomObject(p_direct_move_random_x) * GetGameSize().width;
				val.y = GetRandomObject(p_direct_move_random_y) * GetGameSize().height;

				return val;
			}

			float GetPercentRandom()
			{
				float val = GetRandomObject(p_percentProcessRandom);
				return val;
			}

			float GetTimeProcessRandom()
			{
				float val = GetRandomObject(p_timeProcessRandom * 10);
				return val / 10.f;
			}
		};

		class Background
		{
		protected:
			std::map<RKString, LayerData*> p_ListObject;

			RKString p_Name;
		public:
			Background()
			{
				p_Name = "";
			}
			~Background()
			{
				p_ListObject.clear();
			}

			LayerData * GetLayerData(RKString str)
			{
				if (p_ListObject.size() > 0 && p_ListObject.find(str) != p_ListObject.end())
					return p_ListObject.at(str);
				return 0;
			}

			LayerData * GetLayerDataByIdx(int idx)
			{
				if (p_ListObject.size() > 0)
				{
					int i = 0;
					for (auto it = p_ListObject.begin(); it != p_ListObject.end(); it++)
					{
						if (i == idx)
						{
							return it->second;
						}
						i++;
					}
				}
				return NULL;
			}

			void InsertLayerData(RKString str, LayerData * c)
			{
				p_ListObject.insert(std::pair<RKString, LayerData*>(str, c));
			}

			RKString GetNameBGLayer() { return p_Name; }

			int GetNumberLayer() { return  p_ListObject.size(); }
		};	

		/*
			use for load material dec
		*/

		struct techniqueMaterialDec
		{
		private:
			RKString name;
			RKList<RKString> p_listPass;
		public:
			techniqueMaterialDec()
			{
				name = "";
			}
			techniqueMaterialDec(RKString name_tech)
			{
				name = name_tech;
			}
			virtual ~techniqueMaterialDec()
			{
				p_listPass.Clear();
			}

			RKString GetName() { return name; }
			RKString GetPassName(int idx)
			{
				if (idx >= 0 && (unsigned int)idx < p_listPass.Size())
				{
					return p_listPass.GetAt((unsigned int)idx);
				}
				return "";
			}
			void InsertPass(RKString name)
			{
				p_listPass.Insert(name, 0);
			}
		};

		struct MaterialDec
		{
			RKString Path;
			RKString namespacePath;
			std::map<RKString, techniqueMaterialDec*> ListTech;
		public:
			MaterialDec()
			{
				Path = "";
				namespacePath = "";
			}
			MaterialDec(RKString path, RKString namesp)
			{
				Path = path;
				namespacePath = namesp;
			}
			virtual ~MaterialDec()
			{
				ListTech.clear();
			}

			RKString GetUrlString()
			{
				return Path + "#" + namespacePath;
			}

			techniqueMaterialDec* GetTechiqueByname(RKString name)
			{
				if (ListTech.size() > 0 && ListTech.find(name) != ListTech.end())
				{
					return ListTech.at(name);
				}
				return 0;
			}
			void InsertTechiqueByname(RKString name, techniqueMaterialDec* dec)
			{
				ListTech.insert(std::pair<RKString, techniqueMaterialDec*>(name, dec));
			}

		};

		struct ShaderDec
		{
			RKString p_name;

			RKString p_name_vert_dx;
			RKString p_name_pixel_dx;

			RKString p_name_vert_gl;
			RKString p_name_pixel_gl;

			ShaderDec()
			{
				p_name = "";
				p_name_vert_dx = "";
				p_name_pixel_dx = "";

				p_name_vert_gl = "";
				p_name_pixel_gl = "";
			}
		};
		
		/*
			use for load sound dec
		*/

		struct SoundDec
		{
			RKString name_id;
			RKString path;
			float delay_first;
			int loop;
			RKString type_sound; //sound or music

			SoundDec()
			{
				name_id = "";
				path = "";
				delay_first = 0;
				loop = 1;
				type_sound = "sound";
			}
			virtual ~SoundDec()
			{

			}
		};


		/*
			use for load effect dec
		*/

		struct EffectDec
		{
			RKString name_effect;
			RKString name_source_plist;
			RKString name_source_tex;

			EffectDec()
			{
				name_effect = "";
				name_source_plist = "";
				name_source_tex = "";
			}
		};

		/*
		use for load scene 3d
		*/

		struct Scene3D
		{
			BasicDec * skybox_tex;
			BasicDec * terrain_tex;
		public:

			Scene3D()
			{
				skybox_tex = new BasicDec();
				terrain_tex = new BasicDec();
			}

			virtual ~Scene3D()
			{
				if (skybox_tex)
				{
					delete skybox_tex;
					skybox_tex = nullptr;
				}
				if (terrain_tex)
				{
					delete terrain_tex;
					terrain_tex = nullptr;
				}
			}
		};

		class LoadDataXML
		{
		private:

			static LoadDataXML *		 p_Instance;

			HARDWARD_PROFILE			 p_HProfile;
            
            TextPacker *                            p_texture_packer_xml;

			std::map<RKString, AtlasTexture *>		p_ListTextureXML;

			std::map<RKString, FontDec*>			p_list_font;

			std::map<RKString, Background*>			p_ListBackground;

			std::map<RKString, UIMenuDec*>			p_ListGameUI;

			std::map<RKString, UIMenuWidgetDec*>	p_ListGameUIWidget;

			std::map<RKString, ObjectDec*>			p_listAbstractBasicObject;

			std::map<RKString, MaterialDec*>		p_listMaterial;

			std::map<RKString, ShaderDec *>			p_listShaders;

			std::map<RKString, SoundDec *>			p_listSoundDec;

			std::map<RKString, EffectDec *>			p_listEffectDec;

			std::map<RKString, Wide_ObjectDec *>	p_listWideObject;

			std::map<RKString, Scene3D*>			p_listScene3D;
		public:

			static LoadDataXML *		 GetInstance()
			{
				if (!p_Instance)
				{
					p_Instance = new LoadDataXML();
				}
				return p_Instance;
			}
			LoadDataXML();
			virtual ~LoadDataXML();
			void OnLoadXMLData(STATE_LOAD_XML state_load = STATE_LOAD_XML::LOAD_TEXTURE_XML);
			/*
			-----------------------------------------------------
			*/
            void OnLoadTexurePackerDecXML(TiXmlDocument * objectXMl);
			TextPacker * GetTexturePackerXML();
			void OnLoadTextureDecXML(TiXmlDocument * objectXMl);
			AtlasTexture * GetAtlastTextureByName(RKString name);
			AtlasTexture * GetAtlasTextureByIdx(int idx);
			int	GetNumberTexture() { return p_ListTextureXML.size(); }

			void InsertFontToList(RKString name, FontDec* ddc);
			FontDec* GetFontDecByName(RKString name);
			FontDec* GetFontDecByIdx(int idx);
			int GetNumberFont() { return p_list_font.size(); }
			/*
			-----------------------------------------------------
			*/
			Background * GetBackground(RKString str);
			void InsertBackground(RKString str, Background * c);
			void OnLoadBackgroundDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			UIMenuDec * GetUIByName(RKString str);
			void InsertUIMenuDec(RKString str, UIMenuDec * c);
			void OnLoadUIDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			UIMenuWidgetDec * GetUIWidgetByName(RKString str);
			void InsertUIMenuWidgetDec(RKString str, UIMenuWidgetDec * c);
			void OnLoadUIWidgetDecXML(TiXmlDocument * objectXMl);
			UILayerWidgetDec * OnParseWidgetEntity(TiXmlElement * element_xml, UILayerWidgetDec* parent_widget, UIMenuWidgetDec * UIDec, sound_click_dec * global_id_sound_click);
			TYPE_UI_WIDGET GetUIWidget(RKString name);
			/*
			-----------------------------------------------------
			*/
			MaterialDec * GetMaterialByName(RKString str);
			void InsertMaterialDec(RKString str, MaterialDec * c);

			ShaderDec * GetShaderByName(RKString str);
			void InsertShaderDec(RKString str, ShaderDec * c);

			void OnLoadMaterialDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			ObjectDec * GetBasicByName(RKString str);
			void InsertBasicDec(RKString str, ObjectDec * c);
			void OnLoadListObjectDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			Wide_ObjectDec * GetWideObjectByName(RKString str);
			void InsertWideObjectDec(RKString str, Wide_ObjectDec * c);
			Wide_ObjectDec * OnParseObjectWide(TiXmlElement * element_xml, Wide_ObjectDec* parent_object , Vec2 design_size);
			void OnLoadListWideObjectDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			SoundDec * GetSoundDecByName(RKString str);
			void InsertSoundDec(RKString str, SoundDec * c);
			void OnLoadListSoundDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			EffectDec * GetEffectDecByName(RKString str);
			void InsertEffectDec(RKString str, EffectDec * c);
			void OnLoadListEffectDecXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/
			Scene3D * GetScene3DXMLByName(RKString str);
			void InsertScene3DXML(RKString str, Scene3D * c);
			void OnLoadListScene3DXML(TiXmlDocument * objectXMl);
			/*
			-----------------------------------------------------
			*/

			HARDWARD_PROFILE GetHarwardProfile();

			Vec2 ConvertToVector2(RKString str);
			Color4B ConvertToColor(RKString str);

			BasicDec * GenerateBasicDec(tinyxml2::XMLElement * element, char * name0, ...);
		private:
			void InsertTextureXML(RKString str, AtlasTexture * te);
		};

	}

    #define XMLMgr xml::LoadDataXML::GetInstance()
}
#endif
