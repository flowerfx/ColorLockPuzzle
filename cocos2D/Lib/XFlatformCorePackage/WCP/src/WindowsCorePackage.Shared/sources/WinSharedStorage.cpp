#include "pch.h"

#include "WinShared.h"


#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
#include "FileAgents.h"
#endif

#if defined(OS_WP81)
#include "client.h"
#endif

#ifdef OS_W10
#include "wrl/client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
#if (WCP_TEST_SAMPLE == 1)
	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetAppLocalFolderPath()
	{
		std::wstring swStr = WCPToolkit::Storage::GetAppLocalFolderPath();
		return ref new Platform::String(swStr.c_str());
	}
#endif

	int WCPToolkitWinPhone80CppCallbacks::nativeRemoveFile(Platform::String^ sFilename)
	{
		return RemoveFile(sFilename);
	}
	int WCPToolkitWinPhone80CppCallbacks::nativeRenameFile(Platform::String^ sFilename1, Platform::String^ sFilename2)
	{
		return RenameFile(sFilename1, sFilename2);
	}
}
#endif

namespace WCPToolkit
{

	const std::wstring Storage::GetAppRoamingFolderPath()
	{
		std::wstring str = Utils::Str2Wstr(WCP_NOT_AVAILABLE);

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		auto folder = Windows::Storage::ApplicationData::Current->RoamingFolder;
		auto path = folder->Path;

		str = std::wstring(path->Begin(), path->End());
#endif

		return str;
	}
	const std::wstring Storage::GetAppInstalledFolderPath()
	{
		Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
		Windows::Storage::StorageFolder^ installedLocation = package->InstalledLocation;
		auto path = installedLocation->Path;

		return std::wstring(path->Begin(), path->End());
	}

	const std::wstring Storage::GetAppLocalFolderPath()
	{
		auto folder = Windows::Storage::ApplicationData::Current->LocalFolder;
		auto path = folder->Path;

		return std::wstring(path->Begin(), path->End());
	}


	int Storage::RemoveFile(std::wstring sFilename)
	{
		int nRet = -1;

#if defined(OS_W8)
		//delete file
		DeleteFileAgent ag(sFilename.c_str());
		ag.start();
		concurrency::agent::wait(&ag);

		//read the success value of the operation (true/false)
		bool success = ag.Success();

		if (success == true)
		{
			nRet = 0;
		}
#elif defined(OS_WP8) || defined(OS_WP81)|| defined(OS_W10)
		//it seems the os functions made some freezes on blitz brigade
		//nRet = NATIVE_CALL(nativeRemoveFile)(ref new Platform::String(filename));
		nRet = _wremove(sFilename.c_str());
#endif
		return nRet;
	}


	int Storage::RenameFile(std::wstring sFilename1, std::wstring sFilename2)
	{
		int nRet = -1;

#if defined(OS_W8)
		//open the first file
		OpenFileAgent ag(sFilename1.c_str());
		ag.start();
		concurrency::agent::wait(&ag);

		Windows::Storage::StorageFile^ file1;
		Windows::Storage::StorageFile^ file2;

		if (ag.Success() == true)
		{
			file1 = ag.GetFile();
		}
		else
		{
			//if the file that should be moved can't be opened
			//exit with an error code

			return -1;
		}

		//open the second file
		OpenFileAgent ag2(sFilename2.c_str());
		ag2.start();
		concurrency::agent::wait(&ag2);

		if (ag2.Success() == true)
		{
			file2 = ag2.GetFile();
		}
		else
		{

			//if the second file can't be opened
			//use it's name to move/rename the first file

			//std::wstring wid_str2(filename2);

			//get the second file's name
			//case when path is using '\\'
			int pos = sFilename2.rfind('\\');
			pos += 1;
			std::wstring wid_str3 = std::wstring(sFilename2.begin() + pos, sFilename2.end());

			//case when path is using '/'
			pos = wid_str3.rfind('/');
			pos += 1;
			std::wstring wid_str4 = std::wstring(wid_str3.begin() + pos, wid_str3.end());
			const wchar_t* w_char3 = wid_str4.c_str();

			//move/rename the first file
			MoveFileAgent ag3(file1, w_char3);
			ag3.start();
			concurrency::agent::wait(&ag3);

			//get the success value of the operation
			bool success = ag3.Success();

			if (success == true) return 0;
			else return -1;
		}

		//if the second file exists replace it with the first file
		ReplaceFileAgent ag3(file1, file2);
		ag3.start();
		concurrency::agent::wait(&ag3);

		//get the success value of the operation
		bool success = ag3.Success();

		if (success == true) return 0;

		return -1;
#elif defined(OS_WP8) || defined(OS_WP81) || defined(OS_W10)
		//it seems the os functions made some freezes on blitz brigade
		//nRet = NATIVE_CALL(nativeRenameFile)(ref new Platform::String(filename1), ref new Platform::String(filename2));
		nRet = _wrename(sFilename1.c_str(), sFilename2.c_str());
#endif
		return nRet;
	}

	FILE* Storage::OpenFile(const std::wstring& sFilename, const std::wstring& sMode)
	{
		return _wfopen(sFilename.c_str(), sMode.c_str());
	}

	int Storage::CloseFile(FILE* file)
	{
		return fclose(file);
	}

	bool Storage::DirectoryExists ( const std::wstring & path )
	{
		WIN32_FILE_ATTRIBUTE_DATA  wfad;
		GetFileAttributesExW( path.c_str() ,GetFileExInfoStandard, &wfad);

		if ( wfad.dwFileAttributes == INVALID_FILE_ATTRIBUTES )
			return false;  

		if ( wfad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
			return true;   

		return false;     
	}
 
	std::vector<std::wstring> Storage::GetFileNamesFromDirectory ( const std::wstring & dir)
	{
		std::vector<std::wstring> names; 

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFileExW ( (dir + L"/*").c_str ( ),FindExInfoStandard, &fd ,FindExSearchNameMatch,nullptr,0 );
		
		if ( hFind != INVALID_HANDLE_VALUE )
		{
			do
			{ 
				if ( !( fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
				{
					names.push_back ( fd.cFileName );
				}
			}
			while ( ::FindNextFile ( hFind, &fd ) );

			::FindClose ( hFind );
		}
		return names;
	}

	

};