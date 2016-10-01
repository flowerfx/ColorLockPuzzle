using System.Windows;
using System.Windows.Controls;
using Microsoft.Phone.Controls;


namespace WCPToolkitWinPhone80Bridge
{
    public sealed class WCPToolkitWinPhone80BridgeClass
    {
        static bool sWin8PhoneBridgeLibraryInited = false;
        private static Panel s_pPanel;

        public static string TrueFalseString()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().TrueFalseString();
        }
        public static string GetAdvertisingID()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetAdvertisingID();
        }
        public static string GetDeviceCarrier()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceCarrier();
        }
        public static string GetDeviceLanguage()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceLanguage();
        }
        public static string GetDeviceName()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceName();
        }
        public static string GetDeviceFirmware()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceFirmware();
        }
        public static string GetDeviceManufacturer()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceManufacturer();
        }
        public static float GetDeviceTotalMemory()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceTotalMemory();
        }
        public static int GetAppMemoryUsageLimit()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetAppMemoryUsageLimit();
        }
        public static float GetCurrentMemoryUsage()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetCurrentMemoryUsage();
        }
        public static float GetMemoryWorkingLimit()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetMemoryWorkingLimit();
        }
        public static float GetPeakMemoryUsage()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetPeakMemoryUsage();
        }
        public static float GetFreeRam()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetFreeRam();
        }
        public static float GetFreeDiskSpace()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetFreeDiskSpace();
        }
        public static string GetOSVersion()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetOSVersion();
        }
        public static int GetWindowWidth()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetWindowWidth();
        }
        public static int GetWindowHeight()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetWindowHeight();
        }
        public static string GetDeviceGPUName()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetDeviceGPUName();
        }
        public static int RemoveFile(string sFilename)
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().RemoveFile(sFilename);
        }
        public static int RenameFile(string sFilename1, string sFilename2)
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().RenameFile(sFilename1, sFilename2);
        }
        public static void SetScreenSleepMode(bool bSleep)
        {
            WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().SetScreenSleepMode(bSleep);
        }
        public static bool IsScreenSleepModeOn()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().IsScreenSleepModeOn();
        }
        public static void LaunchBrowser(string sUrl)
        {
            WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().LaunchBrowser(sUrl);
        }
        public static void LaunchMarketplaceDetail()
        {
            WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().LaunchMarketplaceDetail();
        }
        public static void LaunchMarketplaceReview()
        {
            WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().LaunchMarketplaceReview();
        }
        public static void InitIGB(WCPToolkit.IGBTransferParams p)
        {
            var IGBParams = new WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InitParams();
            IGBParams.GameCode = p.GameCode;
            IGBParams.GameVersion = p.GameVersion;
            IGBParams.ClientID = p.ClientID;
            IGBParams.GameOperator = p.GameOperator;
            IGBParams.HDIDFV = p.HDIDFV;
            IGBParams.WAID = p.WAID;
            IGBParams.Language = p.Language;
            IGBParams.AnonymousAccount =p.AnonymousAccount;
            IGBParams.BaseUrl = p.BaseUrl;
            IGBParams.XboxID = p.XboxID;
            IGBParams.UserAge = p.UserAge;
            IGBParams.OsVersion = p.OsVersion;
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().Init(IGBParams);
        }
        public static void ShowCustomerCare()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowCustomerCare();
        }
        public static void ShowCustomerCareWithBanType(int type)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowCustomerCareWithBanType(type);
        }
        public static void ShowForum()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowForum();
        }
        public static void ShowTerms()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowTerms();
        }
        public static void ShowPrivacy()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowPrivacyPolicy();
        }
        public static void ShowNews()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowNews();
        }
        public static void ShowEULA()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowEULA();
        }
        public static void ShowCustomLink(string url)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowInGameBrowserWithUrl(url);
        }
        public static void ShowFacebookShare(bool openExternally)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowFaceBookShare(openExternally);
        }
        public static void ShowFacebookPage(bool openExternally)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().ShowFaceBookPage(openExternally);
        }
        public static string GetFacebookShareLink()
        {
            return WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().
                GetLink(InGameBrowserLibrary.LinkCategory.FACEBOOKSHARE, "");
        }

        public static string GetIGBGameUpdateURL()
        {
            var instance = WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance();
            if(instance.IsInitialized())
            {
                return instance.GetLink(
                WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.LinkCategory.GAMEUPDATE, "");
            }
            return "";
        }
        public static string GetIGBGameReviewURL()
        {
            var instance = WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance();
            if (instance.IsInitialized())
            {
                return instance.GetLink(
                WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.LinkCategory.GAMEREVIEW, "");
            }
            return "";
        }
        public static void IGBGameLanguageChanged(string newGameLanguage)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetGameLanguage(newGameLanguage);
        }
        public static void IGBBaseURLChanged(string baseURL)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetBaseUrl(baseURL);
        }

        public static void IGBUserAgeChanged(int userAge)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetUserAge(userAge);
        }

        public static void IGBUserGenderSet(int gender)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetUserGender(gender);
        }

        public static void IGBAnonymousAccountChanged(string anonymousAccount)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetAnonymousAccount(anonymousAccount);
        }

        public static void IGBFacebookIDChanged(string newFacebookID)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetFacebookId(newFacebookID);
        }

        public static void IGBGlliveAccountChanged(string newGlliveAccount)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetGLiveAccount(newGlliveAccount);
        }

        public static void IGBXboxAccountChanged(string newXboxAccount)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().SetXboxID(newXboxAccount);
        }

        public static void RefreshUnreadNewsNumber()
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().RefreshUnreadNewsNumber();
        }

        public static int GetUnreadNewsNumber()
        { 
            return WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().GetUnreadNewsNumber();
        }

        public static bool IsIGBOnScreen()
        {
            return WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().IsOnScreen();
        }

        public static void IGBControllerEvent(int crt_event, double value)
        {
            WCPToolkitWinPhone80Bridge.InGameBrowserLibrary.InGameBrowser.GetInstance().OnControllerEvent(crt_event, value);
        }

        public static int GetInternetConnectionInfo()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().GetInternetConnectionInfo();
        }
        public static bool TrueFalseBool()
        {
            return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().TrueFalseBool();
        }

        public static bool CheckInitDone(string from)
        {
            if (sWin8PhoneBridgeLibraryInited)
                return true;
            return false;
        }

        #region LiveTiles
      
        private static void AddLiveTile( string pictureName)
        {
			WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().AddLiveTile(pictureName);
        }
		private static void UpdateLiveTile(bool title)
        {
			WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().UpdateLiveTile(title);
        }

		private static void RemoveLiveTiles()
		{
			WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().RemoveLiveTiles();
		}

        #endregion

        #region VirtualKeyboard

        public static void VKTextChanged(object sender, RoutedEventArgs e)
        {
            TextBox txt = ((TextBox)sender);

            if (txt != null)
            {
                string str = txt.Text;
                WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGEVKOnTextChanged(str);
            }
        }
        public static void VKKeyPressed(object sender, RoutedEventArgs e)
        {
            System.Windows.Input.KeyEventArgs ev = (System.Windows.Input.KeyEventArgs)e;
            if (WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGEVKOnKeyPressed(ev.PlatformKeyCode) == true)
            {
                ev.Handled = true;
            }
        }
        public static void VKLostFocus(object sender, RoutedEventArgs e)
        {
            HideVirtualKeyboard();
        }
        public static void ShowVirtualKeyboard(bool bTextBoxVisible, string sInitialText, int nMaxChars, int nType, int nLeft, int nTop, int nWidth, int nHeight, int nTextAlignment)
        {
            TextBox txt = s_pPanel.FindName("WCPVKeyboard") as TextBox;

            if (txt == null)
            {
                txt = new TextBox();
                s_pPanel.Children.Insert(0, txt);
            }

            System.Windows.Input.InputScope scope = new System.Windows.Input.InputScope();
            System.Windows.Input.InputScopeName name = new System.Windows.Input.InputScopeName();
            name.NameValue = (System.Windows.Input.InputScopeNameValue)nType;
            scope.Names.Add(name);
            txt.InputScope = scope;

            Thickness margin = txt.Margin;
            margin.Left = nLeft;
            margin.Top = nTop;
            margin.Right = 0;
            margin.Bottom = 0;
            txt.Margin = margin;
            txt.Width = nWidth;
            txt.Height = nHeight;
            txt.HorizontalAlignment = HorizontalAlignment.Left;
            txt.VerticalAlignment = VerticalAlignment.Top;
            txt.MaxLength = nMaxChars;
            txt.Visibility = Visibility.Visible;

            txt.KeyDown += VKKeyPressed;
            txt.TextChanged += VKTextChanged;
            txt.LostFocus += VKLostFocus;

            if (bTextBoxVisible)
            {
                txt.Opacity = 100;
            }
            else
            {
                txt.Opacity = 0;
            }

            txt.Name = "WCPVKeyboard";
            txt.Text = sInitialText;

            TextAlignment txtalign = (TextAlignment)nTextAlignment;
            txt.TextAlignment = txtalign;

            txt.Select(txt.Text.Length, 0);

            txt.Focus();
        }

        public static void HideVirtualKeyboard()
        {
            TextBox txt = s_pPanel.FindName("WCPVKeyboard") as TextBox;
            if (txt != null)
            {
                s_pPanel.Children.Remove(txt);
            }
        }

        #endregion

        private static int ShowToastNotification(string title, string content, int delay)
		{
			return WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().ShowToastNotification(title, content , delay);
		}

		public static void RemoveToastNotificationsById(int id)
		{
			WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().RemoveToastNotificationsById(id);

		}

        public static void ExitApplication()
        {
            WCPToolkitWinPhone80.WCPToolkitWinPhone80ContainerClass.GetInstance().ExitApplication();
        }

        public static void GetCoppaCountry(string udid)
        {
            //check for internet is already done in c++ 

            System.Threading.Tasks.Task.Run(() =>
            {
                try
                {
                    string urlToPost = "http://iap.gameloft.com/windows/country-operator/?country";

                    System.Net.WebClient client = new System.Net.WebClient();
                    if (udid != "")
                        client.Headers["x-device-id"] = udid;

                    client.DownloadStringCompleted += OnWebClientDownloadStringCompleted;
                    client.DownloadStringAsync((new System.Uri(urlToPost, System.UriKind.Absolute)));
                }
                catch
                {
                    string sDefaultISO = WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeTESTBRIDGEGetDeviceCountry();
                    WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGECoppaCallback(sDefaultISO);
                }
            });
        }
        
        private static void OnWebClientDownloadStringCompleted(object sender, System.Net.DownloadStringCompletedEventArgs e)
        {
            string responseString = "";
            try
            {
                responseString = e.Result;
            }
            catch
            {
                string sDefaultISO = WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeTESTBRIDGEGetDeviceCountry();
                WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGECoppaCallback(sDefaultISO);
            }
            if (responseString.Contains("xml"))
            {
                GetXmlNode(responseString);
            }
        }
        private static void GetXmlNode(string xml)
        {
            using (System.Xml.XmlReader reader = System.Xml.XmlReader.Create(new System.IO.StringReader(xml)))
            {
                try
                {
                    while (reader.Read())
                    {
                        if (reader.NodeType == System.Xml.XmlNodeType.Element)
                        {
                            if (reader.HasAttributes)
                            {
                                try
                                {
                                    if (reader.Name.ToString().Equals("country"))
                                    {
                                        string MCC = reader.GetAttribute("mcc").ToString();
                                        string iso = reader.GetAttribute("iso").ToString().ToUpper();

                                        WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGECoppaCallback(iso);

                                        return;
                                    }
                                }
                                catch
                                {
                                }
                            }
                        }
                    }
                }
                catch
                {
                }
            }

            string sDefaultISO = WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeTESTBRIDGEGetDeviceCountry();

            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.nativeBRIDGECoppaCallback(sDefaultISO);
        }

        #region PushNotifications
        public static bool InitPN()
        {
            return WCPToolkitWinPhone80Bridge.PushNotificationsLibrary.PushNotifications.Init();
        }

        public static void CloseChannelPN()
        {
            WCPToolkitWinPhone80Bridge.PushNotificationsLibrary.PushNotifications.CloseChannel();
        }

        public static string GetEndpointPN()
        {
            return WCPToolkitWinPhone80Bridge.PushNotificationsLibrary.PushNotifications.GetEndpoint();
        }

        public static void RegisterCallbackPN(WCPToolkit.CallbackPackerPN.CallbackPacker packerPN)
        {
            WCPToolkitWinPhone80Bridge.PushNotificationsLibrary.PushNotifications.RegisterCallback(packerPN);
        }

        public static void UnregisterCallbackPN()
        {
            WCPToolkitWinPhone80Bridge.PushNotificationsLibrary.PushNotifications.UnregisterCallback();
        }
        #endregion

        public static void InitWin8PhoneBridgeClass(Panel panel)
        {
            sWin8PhoneBridgeLibraryInited = true;
            s_pPanel = panel;

            InGameBrowserLibrary.InGameBrowser.GetInstance().SetParentPanel(panel);

            GetWindowHeight();
            GetWindowWidth();

            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceCarrier += GetDeviceCarrier;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceLanguage += GetDeviceLanguage;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceName += GetDeviceName;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceFirmware += GetDeviceFirmware;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceManufacturer += GetDeviceManufacturer;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetOSVersion += GetOSVersion;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetFreeRam += GetFreeRam;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceTotalMemory += GetDeviceTotalMemory;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetAppMemoryUsageLimit += GetAppMemoryUsageLimit;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetCurrentMemoryUsage += GetCurrentMemoryUsage;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetPeakMemoryUsage += GetPeakMemoryUsage;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetMemoryWorkingLimit += GetMemoryWorkingLimit;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetFreeDiskSpace += GetFreeDiskSpace;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetWindowWidth += GetWindowWidth;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetWindowHeight += GetWindowHeight;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetDeviceGPUName += GetDeviceGPUName;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RemoveFile += RemoveFile;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RenameFile += RenameFile;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.SetScreenSleepMode += SetScreenSleepMode;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IsScreenSleepModeOn += IsScreenSleepModeOn;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.LaunchBrowser += LaunchBrowser;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.LaunchMarketplaceDetail += LaunchMarketplaceDetail;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.LaunchMarketplaceReview += LaunchMarketplaceReview;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.InitIGB += InitIGB;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowCustomerCare += ShowCustomerCare;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowCustomerCareWithBanType += ShowCustomerCareWithBanType;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowForum += ShowForum;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowTerms += ShowTerms;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowPrivacy += ShowPrivacy;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowNews += ShowNews;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowEULA += ShowEULA;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowCustomLink += ShowCustomLink;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowFacebookShare += ShowFacebookShare;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowFacebookPage += ShowFacebookPage;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetFacebookShareLink += GetFacebookShareLink;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetIGBGameUpdateURL += GetIGBGameUpdateURL;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetIGBGameReviewURL += GetIGBGameReviewURL;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBGameLanguageChanged += IGBGameLanguageChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBBaseURLChanged += IGBBaseURLChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBUserAgeChanged += IGBUserAgeChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBUserGenderSet += IGBUserGenderSet;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBAnonymousAccountChanged += IGBAnonymousAccountChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBFacebookIDChanged += IGBFacebookIDChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBGlliveAccountChanged += IGBGlliveAccountChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBXboxAccountChanged += IGBXboxAccountChanged;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RefreshUnreadNewsNumber+= RefreshUnreadNewsNumber;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetUnreadNewsNumber += GetUnreadNewsNumber;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IsIGBOnScreen += IsIGBOnScreen;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.IGBControllerEvent += IGBControllerEvent;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetInternetConnectionInfo += GetInternetConnectionInfo;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.TrueFalseBool += TrueFalseBool;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetAdvertisingID += GetAdvertisingID;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.TrueFalseString += TrueFalseString;

            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.AddLiveTiles += AddLiveTile;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.UpdateLiveTiles += UpdateLiveTile;
			WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RemoveLiveTiles += RemoveLiveTiles;

			WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowToastNotification += ShowToastNotification;
			WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RemoveToastNotificationsById += RemoveToastNotificationsById;
            #region PushNotifications
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.InitPN += InitPN;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.CloseChannelPN += CloseChannelPN;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetEndpointPN += GetEndpointPN;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.RegisterCallbackPN += RegisterCallbackPN;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.UnregisterCallbackPN += UnregisterCallbackPN;
            #endregion
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ExitApplication += ExitApplication;

            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.GetCoppaCountry += GetCoppaCountry;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.ShowVirtualKeyboard += ShowVirtualKeyboard;
            WCPToolkit.WCPToolkitWinPhone80CppCallbacks.HideVirtualKeyboard += HideVirtualKeyboard;
        }

    }
}
