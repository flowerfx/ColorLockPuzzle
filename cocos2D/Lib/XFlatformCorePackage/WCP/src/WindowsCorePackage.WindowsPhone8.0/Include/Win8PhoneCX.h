#ifndef __Win8Phone_WP8_CALLBACKS_H__
#define __Win8Phone_WP8_CALLBACKS_H__

#include "../WindowsCorePackage.Shared/pch.h"
#include "../../WindowsCorePackage.Shared/PushNotifications/PushNotificationData.h"

namespace WCPToolkit
{
	public delegate Platform::String^	Win8Phone_string_delegate();
	public delegate float				Win8Phone_float_delegate();
	public delegate bool				Win8Phone_bool_string_delegate(Platform::String^ sMovie);
	public delegate int					Win8Phone_int_delegate();
	public delegate int					Win8Phone_int_string_delegate(Platform::String^ sFilename);
	public delegate int					Win8Phone_int_string_string_delegate(Platform::String^ sFilename1, Platform::String^ sFilename2); 
	public delegate int					Win8Phone_int_string_string_int_delegate(Platform::String^ sFilename1, Platform::String^ sFilename2, int integer); 
	public delegate bool				Win8Phone_bool_delegate();
	public delegate void				Win8Phone_void_delegate();
	public delegate void				Win8Phone_void_int_delegate(int id);
	public delegate void                Win8Phone_void_int_double_delegate(int i, double f);
	public delegate void				Win8Phone_void_bool_delegate(bool bSleep);
	public delegate void				Win8Phone_void_string_delegate(Platform::String^ sUrl);
	public delegate bool				Win8Phone_bool_float_delegate(float frequency);
	public delegate void				Win8Phone_void_2str_bool_delegate(Platform::String^ sFilename1, Platform::String^ sFilename2, bool title);
	public delegate void				Win8Phone_void_Packer_delegate(CallbackPackerPN::CallbackPacker^ packerPN);
	public delegate void				Win8Phone_VKeyboard_delegate(bool bTextBoxVisible, Platform::String^ sInitialText, int nMaxChars, int nType, int nLeft, int nTop, int nWidth, int nHeight, int nTextAlignment);

	public ref class IGBTransferParams sealed
	{
	public:
		property int UserAge;
		property Platform::String^ GameCode;
		property Platform::String^ GameVersion;
		property Platform::String^ ClientID;
		property Platform::String^ GameOperator;
		property Platform::String^ HDIDFV;
		property Platform::String^ WAID;
		property Platform::String^ Language;
		property Platform::String^ AnonymousAccount;
		property Platform::String^ BaseUrl;
		property Platform::String^ XboxID;
		property Platform::String^ OsVersion;
	};
	public delegate void				Win8Phone_void_IGBTransferParams_delegate(IGBTransferParams^ p);

	public ref class WCPToolkitWinPhone80CppCallbacks sealed
    {	

	public :
		static property Platform::String^ s_sCoppaCountry;

		//CPP to C# calls
		static event Win8Phone_string_delegate^			GetAdvertisingID;
		static event Win8Phone_string_delegate^			GetDeviceCarrier;
		static event Win8Phone_string_delegate^			GetDeviceLanguage;
		static event Win8Phone_string_delegate^			GetDeviceName;
		static event Win8Phone_string_delegate^			GetDeviceFirmware;
		static event Win8Phone_string_delegate^			GetDeviceManufacturer;
		static event Win8Phone_string_delegate^			GetOSVersion;
		static event Win8Phone_float_delegate^			GetDeviceTotalMemory;
		static event Win8Phone_float_delegate^			GetCurrentMemoryUsage;
		static event Win8Phone_float_delegate^			GetPeakMemoryUsage;
		static event Win8Phone_float_delegate^			GetMemoryWorkingLimit;
		static event Win8Phone_int_delegate^			GetAppMemoryUsageLimit;
		static event Win8Phone_float_delegate^			GetFreeRam;
		static event Win8Phone_float_delegate^			GetFreeDiskSpace;
		static event Win8Phone_int_delegate^			GetWindowWidth;
		static event Win8Phone_int_delegate^			GetWindowHeight;
		static event Win8Phone_string_delegate^			GetDeviceGPUName;
		static event Win8Phone_int_string_delegate^		RemoveFile;
		static event Win8Phone_int_string_string_delegate^	RenameFile;
		static event Win8Phone_bool_delegate^			IsScreenSleepModeOn;
		static event Win8Phone_void_bool_delegate^		SetScreenSleepMode;
		static event Win8Phone_void_string_delegate^	LaunchBrowser;
		static event Win8Phone_void_delegate^			LaunchMarketplaceDetail;
		static event Win8Phone_void_delegate^			LaunchMarketplaceReview;
		static event Win8Phone_void_IGBTransferParams_delegate^     InitIGB;
		static event Win8Phone_void_delegate^           ShowCustomerCare;
		static event Win8Phone_void_int_delegate^       ShowCustomerCareWithBanType;
		static event Win8Phone_void_delegate^           ShowForum;
		static event Win8Phone_void_delegate^           ShowTerms;
		static event Win8Phone_void_delegate^           ShowPrivacy;
		static event Win8Phone_void_delegate^           ShowNews;
		static event Win8Phone_void_delegate^           ShowEULA;
		static event Win8Phone_void_string_delegate^    ShowCustomLink;
		static event Win8Phone_void_bool_delegate^      ShowFacebookShare;
		static event Win8Phone_void_bool_delegate^      ShowFacebookPage;
		static event Win8Phone_string_delegate^         GetFacebookShareLink;
		static event Win8Phone_string_delegate^         GetIGBGameUpdateURL;
		static event Win8Phone_string_delegate^         GetIGBGameReviewURL;
		static event Win8Phone_void_string_delegate^    IGBGameLanguageChanged;
		static event Win8Phone_void_string_delegate^    IGBBaseURLChanged;
		static event Win8Phone_void_int_delegate^       IGBUserAgeChanged;
		static event Win8Phone_void_int_delegate^       IGBUserGenderSet;
		static event Win8Phone_void_string_delegate^    IGBAnonymousAccountChanged;
		static event Win8Phone_void_string_delegate^    IGBFacebookIDChanged;
		static event Win8Phone_void_string_delegate^    IGBGlliveAccountChanged;
		static event Win8Phone_void_string_delegate^	IGBXboxAccountChanged;
		static event Win8Phone_void_delegate^           RefreshUnreadNewsNumber;
		static event Win8Phone_int_delegate^            GetUnreadNewsNumber;
		static event Win8Phone_bool_delegate^           IsIGBOnScreen;
		static event Win8Phone_void_int_double_delegate^ IGBControllerEvent;
		static event Win8Phone_int_delegate^			GetInternetConnectionInfo;
		static event Win8Phone_bool_delegate^			TrueFalseBool;
		static event Win8Phone_string_delegate^			TrueFalseString;
		static event Win8Phone_void_string_delegate^	AddLiveTiles;
		static event Win8Phone_void_bool_delegate^		UpdateLiveTiles; 
		static event Win8Phone_void_delegate^			RemoveLiveTiles;
		static event Win8Phone_void_2str_bool_delegate^	LoadTilesFromPath;
		static event Win8Phone_bool_delegate^			InitPN;
		static event Win8Phone_string_delegate^			GetEndpointPN;
		static event Win8Phone_void_Packer_delegate^	RegisterCallbackPN;
		static event Win8Phone_void_delegate^			UnregisterCallbackPN;
		static event Win8Phone_void_delegate^			CloseChannelPN;
		static event Win8Phone_int_string_string_int_delegate^  ShowToastNotification;
		static event Win8Phone_void_int_delegate^		RemoveToastNotificationsById;
		static event Win8Phone_void_delegate^			ExitApplication;
		static event Win8Phone_void_string_delegate^	GetCoppaCountry;
		static event Win8Phone_VKeyboard_delegate^		ShowVirtualKeyboard;
		static event Win8Phone_void_delegate^			HideVirtualKeyboard;

		static void										nativeGetCoppaCountry(Platform::String^ udid);
		static Platform::String^						nativeGetDeviceCarrier();
		static Platform::String^						nativeGetDeviceLanguage();
		static Platform::String^						nativeGetDeviceName();
		static Platform::String^						nativeGetDeviceFirmware();
		static Platform::String^						nativeGetDeviceManufacturer();
		static Platform::String^						nativeGetOSVersion();
		static float									nativeGetDeviceTotalMemory();
		static float									nativeGetCurrentMemoryUsage();
		static float									nativeGetPeakMemoryUsage();
		static float									nativeGetMemoryWorkingLimit();
		static float									nativeGetFreeRam();
		static float									nativeGetFreeDiskSpace();
		static int										nativeGetWindowWidth();
		static int										nativeGetWindowHeight();
		static Platform::String^						nativeGetDeviceGPUName();
		static int										nativeGetAppMemoryUsageLimit();
		static int										nativeRemoveFile(Platform::String^ sFilename);
		static int										nativeRenameFile(Platform::String^ sFilename1, Platform::String^ sFilename2);
		static void										nativeSetScreenSleepMode(bool bSleep);
		static bool										nativeIsScreenSleepModeOn();
		static void										nativeLaunchBrowser(Platform::String^ sUrl);
		static void										nativeLaunchMarketplaceDetail();
		static void										nativeLaunchMarketplaceReview();
		static void                                     nativeInitIGB(IGBTransferParams^ p);
		static void                                     nativeShowCustomerCare();
		static void                                     nativeShowCustomerCareWithBanType(int type);
		static void                                     nativeShowForum();
		static void                                     nativeShowTerms();
		static void                                     nativeShowPrivacy();
		static void                                     nativeShowNews();
		static void                                     nativeShowEULA();
		static void                                     nativeShowFacebookShare(bool openExternally);
		static void                                     nativeShowFacebookPage(bool openExternally);
		static Platform::String^                        nativeGetFacebookShareLink();                                     
		static void                                     nativeShowCustomLink(Platform::String^ sURL);
		static Platform::String^                        nativeGetIGBGameUpdateURL();
		static Platform::String^                        nativeGetIGBGameReviewURL();
		static void                                     nativeSetIGBGameLanguage(Platform::String^ language);
		static void                                     nativeSetIGBBaseURL(Platform::String^ baseURL);
		static void                                     nativeSetIGBUserAge(int userAge);
		static void                                     nativeSetIGBUserGender(int gender);
		static void                                     nativeSetIGBAnonymousAccount(Platform::String^ anonymousAccount);
		static void                                     nativeSetIGBFacebookID(Platform::String^ facebookID);
		static void	                                    nativeSetIGBGlliveAccount(Platform::String^ glliveAccount);
		static void	                                    nativeSetIGBXboxAccount(Platform::String^ xboxAccount);
		static void	                                    nativeRefreshUnreadNewsNumber();
		static int	                                    nativeGetUnreadNewsNumber();
		static bool                                     nativeIsIGBOnScreen();
		static void                                     nativeIGBControllerEvent(int eventType, double value);
		static int										nativeGetInternetConnectionInfo();
		static bool										nativeTrueFalseBool();
		static Platform::String^						nativeTrueFalseString();
		static Platform::String^						nativeGetAdvertisingID();
		static void										nativeAddLiveTiles ( Platform::String^ pictureName );
		static void										nativeUpdateLiveTiles ( bool title );
		static void										nativeLoadTilesFromPath (Platform::String^ relativePath, Platform::String^ language , bool title);
		static void										nativeRemoveLiveTiles ( );
		static int										nativeShowToastNotification(Platform::String^ title, Platform::String^ content, int delay);
		static void										nativeRemoveToastNotificationsById(int id);
		static void										nativeInitPN ( );
		static Platform::String^						nativeGetEndpointPN();
		static void										nativeRegisterCallbackPN(CallbackPackerPN::CallbackPacker^ packerPN);
		static void										nativeUnregisterCallbackPN();
		static void										nativeCloseChannelPN();
		static void										nativeExitApplication();
		static void										nativeShowVirtualKeyboard(bool bTextBoxVisible, Platform::String^ sInitialText, int nMaxChars, int nType, int nLeft, int nTop, int nWidth, int nHeight, int nTextAlignment);
		static void										nativeHideVirtualKeyboard();

		static void										nativeBRIDGECoppaCallback(Platform::String^ iso);
		static bool										nativeBRIDGEVKOnKeyPressed(int nKeyCode);
		static void										nativeBRIDGEVKOnTextChanged(Platform::String^ sNewText);

#if (WCP_TEST_SAMPLE == 1)
		static Platform::String^						nativeTESTBRIDGEGetProcessorDetails();
		static int										nativeTESTBRIDGEGetNumberOfCpuCores();
		static Platform::String^						nativeTESTBRIDGEGetDeviceGPUName();
		static Platform::String^						nativeTESTBRIDGEGetGraphicsFeatureLevel();
		static Platform::String^						nativeTESTBRIDGEGetAppLocalFolderPath();
		static bool										nativeTESTBRIDGEHasKeyboardInput();
		static bool										nativeTESTBRIDGEHasMouseInput();
		static bool										nativeTESTBRIDGEHasTouchInput();
		//static Platform::String^						nativeGetCurrentHDIDFV();
		static Platform::String^						nativeTESTBRIDGEGetPublisherHostID();
		static Platform::String^						nativeTESTBRIDGEGetCustomDataID(Platform::String^ sIDstring);
		static Platform::String^						nativeTESTBRIDGEGetInternetConnectionInfo();
		static void										nativeTESTBRIDGEAccelerometerStart(unsigned int interval);
		static void										nativeTESTBRIDGEAccelerometerStop();
		static void										nativeTESTBRIDGEAccelerometerUpdate();
		static float									nativeTESTBRIDGEAccelerometerX();
		static float									nativeTESTBRIDGEAccelerometerY();
		static float									nativeTESTBRIDGEAccelerometerZ();

		static void										nativeTESTBRIDGEGyrometerStart(unsigned int interval);
		static void										nativeTESTBRIDGEGyrometerStop();
		static void										nativeTESTBRIDGEGyrometerUpdate();
		static float									nativeTESTBRIDGEGyrometerX();
		static float									nativeTESTBRIDGEGyrometerY();
		static float									nativeTESTBRIDGEGyrometerZ();

		static int										nativeTESTBRIDGEGetSharedSystemMemory();
		static int										nativeTESTBRIDGEGetDedicatedVideoMemory();
		static int										nativeTESTBRIDGEGetDedicatedSystemMemory();
		static Platform::String^						nativeTESTBRIDGEGamePadRefreshA();
		static Platform::String^						nativeTESTBRIDGEGetDeviceCountry();
#endif
    };
}
#endif