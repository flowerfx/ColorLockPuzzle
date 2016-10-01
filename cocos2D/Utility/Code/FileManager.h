#ifndef _FILE_MANAGER_H_
#define _FILE_MANAGER_H_

#if defined OS_IOS
#endif
#include <string>
#include "cocos2d.h"
#if defined WP8 || defined OS_W8
#define DATA_DIR "/Data/"
#elif defined OS_IOS
#define DATA_DIR "Data/"
#else
#define DATA_DIR "Data/"
#endif

#include "RKString_Code/RKString.h"
USING_NS_CC;
using namespace RKUtils;
#define SAVE_NAME "Data.sav"
#define SAVE_GAMEPLAY_NAME "Level.sav"

namespace Utility
{
	namespace File
	{
		//define common ID save
		#define SAVE_STR_NAME			"NameDisplay"
		#define SAVE_STR_COIN			"CurCoin"
		#define SAVE_STR_UDID			"UDID"
		#define	SAVE_STR_FBID			"FacebookID"
		#define	SAVE_STR_GAMECENTERID   "GamecenterID"
		#define	SAVE_STR_GOOGLEID		"GoogleID"
		#define	SAVE_STR_REMOVE_ADS		"RemoveAds"
		#define	SAVE_STR_EN_SOUND		"EnableSound"
		#define	SAVE_STR_EN_MUSIC		"EnableMusic"
        
        #define	SAVE_STR_FB_TOKEN       "FacebookToken"
        
        #define SAVE_STR_RATING         "AppRating"

		void Initialize();
        
        void InitInstallPath();
		FILE * GetFile(RKString namepath, RKString sub);
        FILE * GetLocalFile(RKString namepath, RKString sub);
		RKString ResolveNamePath(RKString namepath);
		RKString ChangeExtensionName(RKString namepath, RKString ex);
		RKString GenerateMipName(RKString namepath);
		RKString ResolveLocalNamePath(RKString namepath);
        
#ifdef OS_IOS
        RKString GetFullPathResourceFile_IOS(std::string filename);
        RKString GetBundlePath_IOS();
        void SetFileUtilsApple();
        RKString GetWritablePath_IOS();
#endif

		class DigiData
		{
		private:
			float data1;
			float data2;
		public:
			DigiData() { data1 = 0.f; data2 = 0.f; }
			DigiData(float p) {
				data1 = ((float)p / 2.f);
				data2 = data1;
			}
			void SetData(int p)
			{
				data1 = ((float)p / 2.f);
				data2 = data1;
			}
			float GetData()
			{
				return (data2 + data1);
			}
		};

		class SaveManager;
		class DataSave
		{
			friend class SaveManager;
		private:
			std::map<RKString, DigiData*> p_listDataSave;

			std::map<RKString, RKString>  p_listCharSave;
		public:
			DataSave() {
				
			}
			~DataSave()
			{
				p_listDataSave.clear();
				p_listCharSave.clear();
			}

			float GetDataByName(RKString name)
			{
				if (p_listDataSave.size() > 0 && p_listDataSave.find(name) != p_listDataSave.end())
				{
					return p_listDataSave.at(name)->GetData();
				}
				return 0.f;
			}

			void SetDataAtName(RKString name , float data)
			{
				if (p_listDataSave.size() > 0 && p_listDataSave.find(name) != p_listDataSave.end())
				{		
					p_listDataSave.erase(name);
					p_listDataSave.insert(std::pair<RKString, DigiData*>(name, new DigiData(data)));
				}
				else if (p_listDataSave.find(name) == p_listDataSave.end() || p_listDataSave.size() <= 0)
				{
					p_listDataSave.insert(std::pair<RKString, DigiData*>(name, new DigiData(data)));
				}
				
			}

			RKString GetcharByName(RKString name)
			{
				if (p_listCharSave.size() > 0 && p_listCharSave.find(name) != p_listCharSave.end())
				{
					return p_listCharSave.at(name);
				}
				return "";
			}

			void SetCharAtName(RKString name, RKString data)
			{
				if (p_listCharSave.size() > 0 && p_listCharSave.find(name) != p_listCharSave.end())
				{
					p_listCharSave.erase(name);
					p_listCharSave.insert(std::pair<RKString, RKString>(name, data));
				}
				else if (p_listCharSave.find(name) == p_listCharSave.end() || p_listCharSave.size() <= 0)
				{
					p_listCharSave.insert(std::pair<RKString, RKString>(name, data));
				}

			}
			
		};


		class SaveManager
		{
		private:
			static SaveManager * m_Instance;
			//
			DataSave *	p_DataSave;
			//
		public:
			static SaveManager * GetInstance()
			{
				if (!m_Instance)
				{
					m_Instance = new SaveManager();
				}
				return m_Instance;
			}

			SaveManager();
			virtual ~SaveManager();

			DataSave * GetDataSave()
			{
				return p_DataSave;
			}


			void SaveTheGameSave();
			void LoadTheGameSave();

			

		protected:

			RKString OnLoadDataSave();
			void OnSaveData(FILE * f);

			RKString CreateBufferSave(bool Reset = false);

			RKString OnEngryptString(RKString in);
			RKString OnDegryptString(RKString in);

			bool OnExtractData(RKString str);

			bool LoadLevelFromBufferData(RKString str);

			bool OnResetFileSile();
			void OnGenerateUserName();

			RKString GetUDID();

		};
#define SaveMgr SaveManager::GetInstance()
	}

}
#endif//_FILE_MANAGER_H_