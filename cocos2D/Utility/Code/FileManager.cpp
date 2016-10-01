#include "FileManager.h"
#include "DisplayView.h"
#ifdef WP8
#include "Direct3DInterop.h"
#include "Windows.h"
#endif
#include <vector>
#include "FXFlatform/XFlatform.h"
extern std::vector<std::string> split(const std::string& s, char seperator);
#include "RKFile_Code/RKFile.h"
using namespace RKUtils;
namespace Utility
{
#if defined WP8 || defined OS_W8
	std::wstring stows(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
		wchar_t *buffer = new wchar_t[size_needed];
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer, size_needed);
		std::wstring wstrTo(buffer);
		delete[]buffer;
		return wstrTo;
	}

	std::string wstos(const std::wstring& wstr)
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		char *buffer = new char[size_needed];
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buffer, size_needed, NULL, NULL);
		std::string strTo(buffer);
		delete[]buffer;
		return strTo;
	}

	Platform::String ^stops(std::string s)
	{
		std::wstring wText = stows(s.c_str());
		return ref new Platform::String(wText.c_str());
	}

	std::wstring pstows(Platform::String^ ps)
	{
		std::wstring wst(ps->Data());
		return wst;
	}

	std::string pstos(Platform::String^ ps)
	{
		std::wstring wst(ps->Data());
		std::string stRet(wst.begin(), wst.end());
		return stRet;
	}

	Platform::String ^atops(const char *text)
	{
		std::wstring wText = stows(text);
		return ref new Platform::String(wText.c_str());
	}

	void pstoa(Platform::String^ ps, char* a_szOutPut)
	{
		std::string stdString = pstos(ps);
		const char* szString = stdString.c_str();
		strcpy(a_szOutPut, szString);
	}

	char * SOCK_API_STRNEW(const char * string)
	{
		if (string == NULL)
		{
			return NULL;
		}
		char * data = NULL;
		int len = strlen(string);

		data = new char[len + 1];
		memset(data, 0, len + 1);
		memcpy(data, string, len);
		return data;
	}
#elif defined OS_IOS
#include "RKFile_Code/RKFile.h"

#endif
	namespace File
	{
		//

		std::string p_gLocalPath;
		std::string p_gInstallPath;
		const char * SecrectKey = "*@#&$FajJD&@342!";
		void Initialize()
		{
#if defined OS_IOS
            SetFileUtilsApple();
#endif
		}
        
        void InitInstallPath()
        {
#if defined WP8 || defined OS_W8
                auto LocalPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
                p_gLocalPath = wstos(LocalPath->Data());
                
                auto InstallPath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
                p_gInstallPath = wstos(InstallPath->Data());
#elif defined OS_IOS
                SetFileUtilsApple();
                p_gLocalPath = GetWritablePath_IOS().GetString();
                p_gInstallPath = GetBundlePath_IOS().GetString();
#elif defined GAME_WIN32
			p_gLocalPath = RKFile_GetDocumentsPath();
			p_gInstallPath = RKFile_GetBundlePath();
#else
                
                p_gLocalPath = "";
                p_gInstallPath = "";
#endif
        }
        
        RKString RecorrectNamePath(RKString namepath)
        {
            if(namepath.Contains("\\"))
            {
                namepath.ReplaceAll("\\", "/");
            }
            
            return namepath;
        }
        
		FILE * GetLocalFile(RKString namepath, RKString sub)
		{
            namepath = RecorrectNamePath(namepath);
            
			RKString finalPath = RKString(p_gLocalPath) + "/" + namepath;
			return  fopen(finalPath.GetString(), sub.GetString());
		}

		FILE * GetFile(RKString namepath, RKString sub)
		{
            namepath = RecorrectNamePath(namepath);
			RKString finalPath = RKString(p_gInstallPath) + DATA_DIR + namepath;
			return fopen(finalPath.GetString(), sub.GetString());
		}

		RKString ResolveNamePath(RKString namepath)
		{
			RKString finalPath = "";
            
        
            namepath = RecorrectNamePath(namepath);
			if (p_gInstallPath != "")
			{
				finalPath = RKString(p_gInstallPath) + DATA_DIR + namepath;
			}
			else
			{
#if defined WP8 || defined OS_W8
				auto InstallPath = Windows::ApplicationModel::Package::Current->InstalledLocation->Path;
				p_gInstallPath = wstos(InstallPath->Data());
#elif defined OS_IOS
                namepath = DATA_DIR + namepath;
				auto fullPath = GetFullPathResourceFile_IOS(namepath.GetString());
				return fullPath;
#endif
				finalPath = RKString(p_gInstallPath) + DATA_DIR + namepath;
			}

			return finalPath;
		}

		RKString ResolveLocalNamePath(RKString namepath)
		{
			std::string finalPath = "";
            namepath = RecorrectNamePath(namepath);
			if (p_gLocalPath != "")
			{
				finalPath = p_gLocalPath + "/" + std::string(namepath.GetString());
			}
			else
			{
#if defined WP8 || defined OS_W8
				auto InstallPath = Windows::Storage::ApplicationData::Current->LocalFolder->Path;
				p_gLocalPath = wstos(InstallPath->Data());
				finalPath = p_gLocalPath + "/" + std::string(namepath.GetString());
#elif defined OS_IOS
				char * p_LocalPath = NULL;
				p_LocalPath = (char *)GetWritablePath_IOS().GetString();
				p_gLocalPath = p_LocalPath;
				finalPath = p_gLocalPath + namepath.GetString();
#endif
			}
			return finalPath;
		}

		RKString GenerateMipName(RKString namepath)
		{
			auto strs = namepath.Split(".");
			return strs[0] + "_mip." + strs[1];
		}

		RKString ChangeExtensionName(RKString namepath, RKString ex)
		{
			auto strs = namepath.Split(".");
			RKString re_string = "";
			re_string = strs[0] + ex;
			return re_string;
		}
		///////////////////////////////////////////////////////////////

		SaveManager * SaveManager::m_Instance = NULL;

		SaveManager::SaveManager()
		{
			p_DataSave = new DataSave();
		}
		SaveManager::~SaveManager()
		{
			delete p_DataSave;
			p_DataSave = NULL;
		}

		void SaveManager::SaveTheGameSave()
		{
#if defined OS_W8 || defined WP8 || defined OS_IOS
			FILE * f = GetLocalFile(SAVE_NAME, "wb");
#else
			FILE * f = GetFile(SAVE_NAME, "wb");
#endif
			if (!f)
			{
				CCASSERT(false, "nodata to save!");
				return;
			}
			OnSaveData(f);
			fclose(f);
		}

		void SaveManager::LoadTheGameSave()
		{
			auto str = OnLoadDataSave();
			if (str != "")
			{
				bool res = OnExtractData(str);
				if (!res)
				{
					//reset file save if save file is valid;
					FILE * f = GetLocalFile(SAVE_NAME, "wb");
					RKString buffer = CreateBufferSave(true);
					RKString engrypt_buffer = OnEngryptString(buffer);
					fputs(engrypt_buffer.GetString(), f);
					fclose(f);
				}
			}
		}
		
		RKString SaveManager::OnLoadDataSave()
		{
			int file_exist = 1;
#if defined OS_W8 || defined WP8 || defined OS_IOS
			FILE * f = GetLocalFile(SAVE_NAME, "rb"); //read the file in local path
#else
			FILE * f = GetFile(SAVE_NAME, "rb");
#endif
			if (!f) //the file not exist mean that the file is remove or this this the first game play
			{
#if defined OS_W8 || defined WP8 || defined OS_IOS || defined _WIN32
				f = GetLocalFile(SAVE_NAME , "wb"); //create a new file
#else
				f = GetFile(SAVE_NAME, "wb");
#endif
				file_exist = 0;
			}
			if (!f)
			{
#ifndef OS_IOS
				CCASSERT(false, "error in load file save");
#endif
				return "";
			}
			//
			char * buffer_file = new char[1024];
			RKString buffer_data = "";
			if (file_exist == 0)
			{
				p_DataSave->SetDataAtName(SAVE_STR_COIN, 60); //add default 60 coin to game at first
				p_DataSave->SetCharAtName(SAVE_STR_UDID, GetUDID());
                p_DataSave->SetCharAtName(SAVE_STR_EN_MUSIC, "1");
                p_DataSave->SetCharAtName(SAVE_STR_EN_SOUND, "1");
				OnSaveData(f);
			}
			else
			{
				if (fgets(buffer_file, 1024, f))
				{
					buffer_data = OnDegryptString(std::string(buffer_file));
				}
			}
			//
			fclose(f);
			return buffer_data;

		}
		
		void SaveManager::OnSaveData(FILE * f)
		{
			RKString buffer = CreateBufferSave(false);
			RKString engrypt_buffer = OnEngryptString(buffer);
			fputs(engrypt_buffer.GetString(), f);

		}
		
		RKString SaveManager::OnEngryptString(RKString in)
		{
			std::string out = "";
			int idx = 0;
			std::string t_in = in.GetString();
			int sizeOfSecrect = sizeof(SecrectKey) / sizeof(char);
			for (int i = 0; i < t_in.size(); i++)
			{
				char temp = (t_in[i] + SecrectKey[idx]);
				out += temp;
				idx++;
				if (idx >= sizeOfSecrect)
				{
					idx = 0;
				}
			}

			return out;
		}
		
		RKString SaveManager::OnDegryptString(RKString in)
		{
			std::string out = "";
			int idx = 0;
			std::string t_in = in.GetString();
			int sizeOfSecrect = sizeof(SecrectKey) / sizeof(char);
			for (int i = 0; i < t_in.size(); i++)
			{
				char temp = (t_in[i] - SecrectKey[idx]);
				out += temp;
				idx++;
				if (idx >= sizeOfSecrect)
				{
					idx = 0;
				}
			}

			return out;
		}
		
		RKString SaveManager::CreateBufferSave(bool Reset)
		{
			OnGenerateUserName();
			//
			RKString buffer_write_to = "";

			buffer_write_to += "Device Type:";
#if defined W8 || defined WP8
			buffer_write_to += "WP8";
#elif defined WIN32
			buffer_write_to += "WIN32";
#elif defined IOS
			buffer_write_to += "IOS";
#elif defined ANDROID
			buffer_write_to += "ANDROID";
#endif

			for (auto it = p_DataSave->p_listCharSave.begin(); it != p_DataSave->p_listCharSave.end(); it++)
			{
				buffer_write_to += "#";
				buffer_write_to += it->first;
				buffer_write_to += ":";
				buffer_write_to += it->second;
			}

			for (auto it = p_DataSave->p_listDataSave.begin(); it != p_DataSave->p_listDataSave.end(); it++)
			{
				buffer_write_to += "#";
				buffer_write_to += it->first;
				buffer_write_to += ":";
				buffer_write_to += std::to_string(it->second->GetData());
			}

			return buffer_write_to;
		}
		
		void SaveManager::OnGenerateUserName()
		{
			if (p_DataSave->GetcharByName(SAVE_STR_NAME) != "")
				return;
			RKString name = "";
			int random = GetRandom(0, 10);
			if (random == 0)
			{
				name = "User_";
			}
			else if (random == 1)
			{
				name = "Name_";
			}
			else if (random == 2)
			{
				name = "Tag_";
			}
			else if (random == 3)
			{
				name = "Player_";
			}
			else if (random == 4)
			{
				name = "Guy_";
			}
			else if (random == 5)
			{
				name = "Gamer_";
			}
			else if (random == 6)
			{
				name = "NameTag_";
			}
			else if (random == 7)
			{
				name = "Gosu_";
			}
			else if (random == 8)
			{
				name = "Handsome_";
			}
			else if (random == 9)
			{
				name = "Bunny_";
			}
			else if (random == 10)
			{
				name = "Jumper_";
			}

			p_DataSave->SetCharAtName(SAVE_STR_NAME, name);
		}

		bool SaveManager::OnExtractData(RKString str)
		{
			bool res = true;

			auto ListString = split(str.GetString(), '#');
			for (int i = 0; i < ListString.size(); i++)
			{
				auto detailStr = split(ListString[i], ':');
				if (detailStr[0] == "Device Type")
				{
					std::string type = "";
#if defined W8 || defined WP8
					type= "WP8";
#elif defined WIN32
					type = "WIN32";
#elif defined IOS
					type= "IOS";
#elif defined ANDROID
					type = "ANDROID";
#endif
					if (type != detailStr[1])
					{
						return false;
					}
				}
				else 
				{
					if (RKString(detailStr[0]) == RKString(SAVE_STR_UDID) &&
						RKString(detailStr[1]) != GetUDID())
					{
						return false;
					}

						float __Data = atof(detailStr[1].c_str());
						if (std::to_string(__Data) == detailStr[1]) //covert succeed
						{
							p_DataSave->SetDataAtName(detailStr[0], __Data);
						}
						else
						{
							p_DataSave->SetCharAtName(detailStr[0], detailStr[1]);
						}

				}			
			}

			return res;
		}
		
		RKString SaveManager::GetUDID()
		{
			return XFlatform::GetUDID();
		}
		//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////
		
		bool SaveManager::LoadLevelFromBufferData(RKString str)
		{
			bool res = true;

			auto ListString = split(str.GetString(), '#');
			auto detailStr = split(ListString[0], ':');
			if (detailStr[0] == "Device Type")
			{
				std::string type = "";
#if defined OS_W8 || defined WP8
				type = "WP8";
#elif defined WIN32
				type = "WIN32";
#elif defined GAME_IOS
				type = "IOS";
#elif defined GAME_ANDROID
				type = "ANDROID";
#endif
				if (type != detailStr[1])
				{
					return false;
				}
			}
			detailStr = split(ListString[1], ':');
			if (detailStr[0] == "DeviceID")
			{
				RKString id = detailStr[1];
				if (id != GetUDID())
				{
					return false;
				}
			}

			//
			
			return res;

		}

		
	}
}