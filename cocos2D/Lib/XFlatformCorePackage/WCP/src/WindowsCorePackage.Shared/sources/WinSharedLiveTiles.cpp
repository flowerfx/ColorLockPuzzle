#include "pch.h"
#include "WinShared.h"

#if defined(OS_WP81)
#include "client.h"
#endif
#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{

	void WCPToolkitWinPhone80CppCallbacks::nativeAddLiveTiles ( Platform::String^ pictureName )
	{
		AddLiveTiles ( pictureName );
	}
	void WCPToolkitWinPhone80CppCallbacks::nativeUpdateLiveTiles ( bool title )
	{
		UpdateLiveTiles ( title );
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeLoadTilesFromPath ( Platform::String^ relativePath, Platform::String^ language, bool title )
	{
		if ( title )
		{
			LiveTiles::LoadTilesFromPath ( std::wstring ( relativePath->Begin ( ) ), std::wstring ( language->Begin ( ) ), WCPToolkit::LiveTiles::Title );
		}
		else
		{
			LiveTiles::LoadTilesFromPath ( std::wstring ( relativePath->Begin ( ) ), std::wstring ( language->Begin ( ) ), WCPToolkit::LiveTiles::None );
		}
	}

	void WCPToolkitWinPhone80CppCallbacks::nativeRemoveLiveTiles()
	{ 
		RemoveLiveTiles();
	}

}
#endif

namespace WCPToolkit
{




#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	static  Windows::UI::Notifications::TileUpdater^ s_TileNotificationUpdater = nullptr;
#endif

	void LiveTiles::LoadTilesFromPath ( const std::wstring & path, const std::wstring & language, TileBranding branding /*= None*/ )
	{
		if ( path.empty ( ) )
		{
			WCPToolkit::Utils::Log(L"ERROR - path cannot be empty");
			return;
		}


		std::wstring installPath = WCPToolkit::Storage::GetAppInstalledFolderPath ( );

		std::wstring absPath;
		std::wstring absPath_lang;

		if ( path.back ( ) == L'/' || path.back ( ) == L'\\' )
		{
			absPath = path + L"default\\";
			absPath_lang = path + language + L"Tiles\\";
		}
		else
		{
			absPath = path + L"\\default\\";
			absPath_lang = path + L"\\" + language + L"Tiles\\";
		}

		std::wstring relativePath_default = absPath.substr ( installPath.size ( ), absPath.size ( ) );
		std::wstring relativePath_lang = absPath_lang.substr ( installPath.size ( ), absPath_lang.size ( ) );


		if ( !Storage::DirectoryExists ( absPath ) )
		{
 		 	WCPToolkit::Utils::Log(L"ERROR - default directory for default tiles don`t exists");
			return;
		}

		bool dir_lang = true;

		if ( !Storage::DirectoryExists ( absPath_lang ) )
		{
			dir_lang = false; 
			std::wstring log = L"Warning - directory for language: " + language + L"don`t exists! \n If you don`t have tiles for another language it`s OK!";
 			WCPToolkit::Utils::Log(log);


		}

		std::vector<std::wstring> fileNames_default;
		std::vector<std::wstring> fileNames_lang;

		fileNames_default = Storage::GetFileNamesFromDirectory ( absPath );

		if ( dir_lang )
		{
			fileNames_lang = Storage::GetFileNamesFromDirectory ( absPath_lang );
		}
		 
		if ( fileNames_default.size ( ) == 0 )
		{
			WCPToolkit::Utils::Log(L"ERROR - No images for tile in directory!");
			return;
		}
		std::map<std::wstring, std::wstring> map;
		std::map<std::wstring, std::wstring>::iterator it;


		for ( auto & f : fileNames_default )
		{
			map[f] = relativePath_default;
		}

		if ( fileNames_lang.size ( ) > 0 )
		{
			for ( auto & f_lang : fileNames_lang )
			{
				it = map.find ( f_lang );

				if ( it != map.end ( ) )
				{
					it->second = relativePath_lang;
				}
				else
				{
					map[f_lang] = relativePath_lang;
				}
			}
		}



#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		std::wstring tileXmlString = L"<list><tile><visual version='3'>";
		std::wstring previd = L"1";

		for ( auto & f : map )
		{
			std::wstring ss = f.first;

#ifdef OS_W10
			wchar_t* wcstok_context;
			std::wstring currentid = std::wcstok ( const_cast< wchar_t* >( ss.c_str ( ) ), L"_", &wcstok_context);
#else
			std::wstring currentid = std::wcstok(const_cast< wchar_t* >(ss.c_str()), L"_");
#endif

			std::wstring covertedPath = f.second;
			std::replace ( covertedPath.begin ( ), covertedPath.end ( ), '\\', '/' );

			if ( 0 != currentid.compare ( previd ) )
			{
				previd = currentid;
				tileXmlString += L"</visual></tile>";
				tileXmlString += L"<tile> <visual  version='3'>";
			}

			if ( f.first.find ( L"largetile" ) != -1 )
			{
				tileXmlString += L"<binding template='TileSquare310x310Image' ";
			}
			else if ( f.first.find ( L"mediumtile" ) != -1 )
			{
				tileXmlString += L"<binding template='TileSquare150x150Image' ";
			}
			else if ( f.first.find ( L"widetile" ) != -1 )
			{
				tileXmlString += L"<binding template='TileWide310x150Image' ";
			}
			else if ( f.first.find ( L"smalltile" ) != -1 )
			{
				tileXmlString += L"<binding template='TileSquare71x71Image' ";
			}

			switch ( branding )
			{
				case WCPToolkit::LiveTiles::None: tileXmlString += L"branding='none'>";
					break;
				case WCPToolkit::LiveTiles::Logo: tileXmlString += L"branding='logo'>";
					break;
				case WCPToolkit::LiveTiles::Title: tileXmlString += L"branding='name'>";
					break;

			}


			tileXmlString += L"<image id='1' src='ms-appx://" + covertedPath + f.first + L"' alt='' />";
			tileXmlString += L"</binding>";

		}

		tileXmlString += L"</visual></tile></list>";

		 		 
		Windows::Data::Xml::Dom::XmlDocument^ tileDOM = ref new Windows::Data::Xml::Dom::XmlDocument ( );

		tileDOM->LoadXml ( ref new Platform::String ( tileXmlString.c_str ( ) ) );

		Windows::Data::Xml::Dom::XmlNodeList^ list = tileDOM->GetElementsByTagName ( "tile" );

		if ( nullptr == s_TileNotificationUpdater )
			s_TileNotificationUpdater = Windows::UI::Notifications::TileUpdateManager::CreateTileUpdaterForApplication ( );

		s_TileNotificationUpdater->Clear ( );
		s_TileNotificationUpdater->EnableNotificationQueue ( true );

		if ( list->Length != 0 )
		{
			for ( auto it = list->First ( ); it->HasCurrent; it->MoveNext ( ) )
			{
				Windows::Data::Xml::Dom::XmlDocument^ tileXml = ref new Windows::Data::Xml::Dom::XmlDocument;
				tileXml->LoadXml ( it->Current->GetXml ( ) );

				Windows::UI::Notifications::TileNotification^ tile = ref new Windows::UI::Notifications::TileNotification ( tileXml );

				s_TileNotificationUpdater->Update ( tile );
			}

		}


#elif defined(OS_WP8)  
		for ( auto & f : map )
		{
			NATIVE_CALL ( nativeAddLiveTiles )( ref new Platform::String ( ( f.second + f.first).c_str ( ) ) );
		}

	 
		NATIVE_CALL ( nativeUpdateLiveTiles )( (( branding == WCPToolkit::LiveTiles::Title)?true:false) );
		 
#endif


		}
	void LiveTiles::RemoveLiveTiles()
	{
#if defined(OS_W8) || defined(OS_W10)
		if ( nullptr == s_TileNotificationUpdater )
			s_TileNotificationUpdater = Windows::UI::Notifications::TileUpdateManager::CreateTileUpdaterForApplication ( );

		s_TileNotificationUpdater->Clear ( );
#elif OS_WP8
		NATIVE_CALL ( nativeRemoveLiveTiles )();
#endif
	}
};