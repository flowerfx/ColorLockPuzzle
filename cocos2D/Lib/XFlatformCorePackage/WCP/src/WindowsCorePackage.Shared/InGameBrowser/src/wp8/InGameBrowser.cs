using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using Microsoft.Phone.Controls;

using Windows.Phone.Storage.SharedAccess;

using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace InGameBrowserLibrary
{
    public delegate void UnreadNewsChangedCallback(int unreadNewsNumber);
    //public delegate void InGameRedirectCallback(string arg);

    public sealed class InGameBrowser
    {
        private static InGameBrowser m_Instance = null;

        private WebBrowser mWebView = null;
        private Image mBackgroundImage = null;
        private Image mCloseImage = null;
        private Image mBackImage = null;
        private Image mForwardImage = null;
        private Image mReloadImage = null;
        private Panel mPanel = null;
        private ProgressBar prog_ring = null;
        private bool inited_navig = false;
        private static string s_URL = null;

        private string INGAMEBROWSER_URL = "https://201205igp.gameloft.com/redir/ingamebrowser.php";
        private string INGAMEBROWSER_POST_TEMPLATE = "from=FROM&op=OPERATOR&country=COUNTRY&lg=LANG&udid=UDID&game_ver=VERSION&d=DEVICE&f=FIRMWARE&anonymous=ANONYMOUS_ACCOUNT&fbid=FACEBOOK_ID&gliveusername=GLIVE_USERNAME&os=winp&hdidfv=HDIDFV";

        private string FORUM_URL = "https://201205igp.gameloft.com/redir/?";
        private string FORUM_PARAMS_TEMPLATE = "from=FROM&op=OPERATOR&ctg=FORUM&ver=VERSION&lg=LANG&country=COUNTRY&d=DEVICE&f=FIRMWARE&udid=UDID&hdidfv=HDIDFV";

        private string CHECK_UNREAD_NEWS_NUMBER = "https://201205igp.gameloft.com/redir/ingamenews.php?action=checkNews&last-id=LAST_ID";
        private string SAVE_NEWS_ID = "https://201205igp.gameloft.com/redir/ingamenews.php?action=saveNews&last-id=LAST_ID";
        private string DISPLAY_NEWS = "https://201205igp.gameloft.com/redir/ingamenews.php?action=displayNews";

        private int languageIndex = 0;
        private string anonymousAccount = "";
        private string facebookID = "";
        private string gliveAccount = "";
        private string gameCode = "";
        private string gameOperator = "";
        private string gameVersion = "";

        private string HDIDFV = "";
        private string CTG_TYPE = "";

        private static int unreadNewsNumber = -1;
        private static int _lastUnreadNewsIndex = -1;

        private UnreadNewsChangedCallback unreadCallback = null;
        //private InGameRedirectCallback redirectCallback = null;

        private static string[] TXT_IGB_LANGUAGES = { "EN", "FR", "DE", "IT", "SP", "JP", "KR", "CN", "BR", "RU", "TR", "AR", "TH", "ID", "VI", "ZT" };
	
	    private string[] TXT_NET_ERROR = { 
			    "No internet connection available. Please make sure your device is connected to the internet.",
			    "Aucune connexion Internet disponible. Veuillez vous assurer que votre appareil est connecté à Internet", 
			    "Keine Internetverbindung verfügbar. Versichere dich bitte, dass dein Gerät mit dem Internet verbunden ist.",
			    "Nessuna connessione disponibile. Assicurati che il dispositivo sia connesso a internet.", 
			    "No hay ninguna conexión a internet disponible. Por favor, asegúrate de que tu dispositivo esté conectado a internet.",
			    "インターネット接続を利用できません\nデバイスがインターネットに接続されているかご確認ください", 
			    "인터넷에 연결할 수 없습니다. 장치가 인터넷에 연결되어 있는지 확인하세요.",
			    "无可用网络连接。请确保您的设备已连入网络。", 
			    "Conexão indisponível. Certifique-se de que seu aparelho esteja conectado à internet.",
			    "Нет соединения с Интернетом. Пожалуйста, удостоверьтесь, что ваше устройство подключено к Интернету.", 
			    "İnternet bağlantısı yok. Lütfen cihazının internete bağlı olduğundan emin ol.",
			    "لا يوجد اتصال بالإنترنت. يُرجى التحقق من أن جهازك متصل بالإنترنت.", 
			    "ไม่พบ|การเชื่อมต่อ|อินเทอร์เน็ต |กรุณา|ตรวจสอบ|ว่า|เครื่อง|ของคุณ|ได้|เชื่อมต่อ|กับ|อินเทอร์เน็ต",
			    "Koneksi Internet saat ini tidak tersedia. Pastikan perangkatmu sudah terhubung dengan Internet.",
			    "Không có kết nối Internet. Xin đảm bảo thiết bị của bạn đã kết nối Internet.",
			    "无可用网络连接。请确保您的设备已连入网络。"
		    };

	    private string[] TXT_OK = { 
			    "OK",
                "OK",
                "OK",
                "OK",
                "Acep.",
                "OK",
                "OK",
                "OK",
                "OK",
                "OK",
                "OK",
	            "موافق",//AR
	            "ตกลง",//TH
	            "OK",//ID
	            "OK",//VI
	            "OK",//ZT
		};

        private string solveTemplate (string template) 
        {
		    string result = template;
		
		    if (template != null) 
            {
			    if (languageIndex < 0 || languageIndex >= TXT_IGB_LANGUAGES.Length)
				    languageIndex = 0;
			
			    result = result.Replace("FROM", gameCode)
                        .Replace("OPERATOR", gameOperator)
                        .Replace("UDID", getUDID())
                        .Replace("COUNTRY", getCountry())
                        .Replace("LANG", TXT_IGB_LANGUAGES[languageIndex])
                        .Replace("VERSION", gameVersion)
                        .Replace("DEVICE", getDevice())
                        .Replace("FIRMWARE", getFirmware())
                        .Replace("ANONYMOUS_ACCOUNT", anonymousAccount)
                        .Replace("FACEBOOK_ID", facebookID)
                        .Replace("GLIVE_USERNAME", gliveAccount)
                        .Replace("HDIDFV", HDIDFV)
                        .Replace(" ", "");
		    } else return "";
		
		    return result;
	    }

        private string getUDID()
        {
            return Encryptor.Encrypt_MD5(Windows.Phone.System.Analytics.HostInformation.PublisherHostId);
        }

        private string getFirmware()
        {
            return System.Environment.OSVersion.Version.ToString();
        }

        private string getDevice()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceManufacturer + "_" + Microsoft.Phone.Info.DeviceStatus.DeviceName;
        }

        private string getCountry()
        {
            return System.Globalization.RegionInfo.CurrentRegion.TwoLetterISORegionName.ToUpper();
        }

        public void SetUnreadNewsChangedCallback(UnreadNewsChangedCallback callback)
        {
            unreadCallback = callback;
        }

        public void SetLanguageIndex(int index)
        {
            languageIndex = index;
        }

        public void SetAnonymousAccount(string account)
        {
            anonymousAccount = account;
        }

        public void SetFacebookId(string id)
        {
            facebookID = id;
        }

        public void SetGLiveAccount(string account)
        {
            gliveAccount = account;
        }

        public void SetGameCode(string code)
        {
            gameCode = code;
        }

        public void SetGameOperator(string op)
        {
            gameOperator = op;    
        }

        public void SetGameVersion(string version)
        {
            gameVersion = version;
        }

        public void SetHDIDFV(string sHDIDFV)
        {
            HDIDFV = sHDIDFV;
        }


        public static InGameBrowser GetInstance()
        {
            if (m_Instance == null)
                m_Instance = new InGameBrowser();

            return m_Instance;
        }

        public void SetParentPanel(Panel parent)
        {
            mPanel = parent;

            mPanel.SizeChanged += mPanel_SizeChanged;
        }

        void mPanel_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            mPanel.Dispatcher.BeginInvoke(
                delegate
                {
                    double header_height = (0.1 * mPanel.ActualHeight < 50) ? mPanel.ActualHeight * 0.1 : 50;

                    if (mBackgroundImage != null && mWebView != null && prog_ring != null)
                    {
                        mBackgroundImage.Width = mPanel.ActualWidth;
                        mBackgroundImage.Height = mPanel.ActualHeight;
                        mWebView.Width = mPanel.ActualWidth;
                        mWebView.Height = mPanel.ActualHeight - header_height * 2;
                        prog_ring.Margin = new Thickness(0, header_height, 0, 0);
                        prog_ring.Width = mPanel.ActualWidth;
                    }
                }
            );
        }

        public void ShowCustomerCare()
        {
            if (mPanel != null)
            {
                mPanel.Dispatcher.BeginInvoke(
                    delegate
                    {
                        Show("SUPPORT", true);
                    }
                );
            }
        }

        public void ShowCustomerCareWithBanType(int ban_type)
        {
            if (mPanel != null)
            {
                mPanel.Dispatcher.BeginInvoke(
                    delegate
                    {
                        Show("BANNED:" + ban_type, true);
                    }
                );
            }
        }

        public void ShowInGameBrowserWithUrl(string url)
        {
            if (mPanel != null)
            {
                mPanel.Dispatcher.BeginInvoke(
                    delegate
                    {
                        Show("url:" + url, true);
                    }
                );
            }
        }

        public void ShowForum()
        {
            if (mPanel != null)
            {
                mPanel.Dispatcher.BeginInvoke(
                    delegate
                    {
                        Show("FORUM", true);
                    }
                );
            }
        }

        public void ShowNews()
        {
            if (mPanel != null)
            {
                mPanel.Dispatcher.BeginInvoke(
                    delegate
                    {
                        Show("NEWS", true);
                    }
                );
            }
        }

        public void RefreshUnreadNewsNumber()
        {
            RefreshUnreadNewsNumberHelper();
        }

        private async Task RefreshUnreadNewsNumberHelper()
        {
            _debug("RefreshUnreadNewsNumber()");

            string lastNewsIndex = GetLastNewsIndex();
            string serverUrl = CHECK_UNREAD_NEWS_NUMBER.Replace("LAST_ID", lastNewsIndex);
            string data = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);

            //_debug("connecting to " + serverUrl + "\nwith post data:\n" + data);

            string response = await postData(serverUrl, data);

            _debug("response received: " + response);

            try
            {
                JObject json = JObject.Parse(response);
                SaveJsonData(json, false);
            }
            catch(JsonReaderException) { }
        }

        private async Task RefreshUnreadNewsNumberInternal()
        {
            _debug("RefreshUnreadNewsNumberInternal()");

            string lastNewsIndex = GetLastNewsIndex();
            string serverUrl = SAVE_NEWS_ID.Replace("LAST_ID", lastNewsIndex);
            string data = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);

            //_debug("connecting to " + serverUrl + "\nwith post data:\n" + data);

            string response = await postData(serverUrl, data);

            _debug("response received: " + response);

            try
            {
                JObject json = JObject.Parse(response);
                SaveJsonData(json, true);
            }
            catch (JsonReaderException e) { }
        }

        private void SaveJsonData(JObject json, bool saveNumber)
        {
            bool success = json.Value<bool>("success");
            if (success)
            {
                if (saveNumber)
                {
                    string lastNewsIndex = json.Value<string>("current-id");
                    if (lastNewsIndex == null) 
                        lastNewsIndex = "-1";

                    SaveLastNewsIndex(lastNewsIndex);
                }

                if (json["unread"] != null)
                {
                    unreadNewsNumber = (int)json.Value<int>("unread");
                }
                else
                {
                    unreadNewsNumber = -1;
                }

                if (unreadCallback != null)
                {
                    unreadCallback(unreadNewsNumber);
                }
            }
        }

        public int GetUnreadNewsNumber()
        {
            return unreadNewsNumber;
        }

        private void SaveLastNewsIndex(string lastNewsIndex)
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;

            if (settings.Contains("InGameBrowser_lastNewsIndex"))
                settings.Remove("InGameBrowser_lastNewsIndex");

            settings.Add("InGameBrowser_lastNewsIndex", lastNewsIndex);
        }

        private string GetLastNewsIndex()
        {
            IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;

            if (settings.Contains("InGameBrowser_lastNewsIndex"))
                return IsolatedStorageSettings.ApplicationSettings["InGameBrowser_lastNewsIndex"] as string;
            else
                return "-1";
        }

        private Task<string> postData(string url, string paramz)
        {
            _debug("postData making post at: " + url + "\n" + paramz);

            TaskCompletionSource<string> tcs = new TaskCompletionSource<string>();
            WebClient client = new WebClient();
            client.Headers[HttpRequestHeader.ContentType] = "application/x-www-form-urlencoded";
            client.UploadStringCompleted += (s, e) =>
            {
                if (e.Error == null)
                {
                    tcs.SetResult(e.Result);
                    //_debug("postData response: " + e.Result);
                }
                else
                {
                    tcs.SetException(e.Error);
                    //_debug("postData error: " + e.Error);
                }
            };
            client.UploadStringAsync(new Uri(url, UriKind.Absolute), paramz);

            return tcs.Task;
        }

        private void Show(string ctg, bool show)
        {
            if (show)
            {
                CTG_TYPE = ctg;

                double header_height = (0.1 * mPanel.ActualHeight < 50) ? mPanel.ActualHeight * 0.1 : 50;

                mBackgroundImage = new Image();
                mBackgroundImage.Source = new BitmapImage(new Uri("Resources/igb_background.png", UriKind.Relative));
                mBackgroundImage.Width = mPanel.ActualWidth;
                mBackgroundImage.Height = mPanel.ActualHeight;
                mBackgroundImage.Stretch = Stretch.Fill;

                mPanel.Children.Add(mBackgroundImage);

                mCloseImage = new Image();
                mCloseImage.Margin = new Thickness(10, 0, 0, 0);
                mCloseImage.Source = new BitmapImage(new Uri("Resources/igb_cancel.png", UriKind.Relative));
                mCloseImage.Width = header_height;
                mCloseImage.Height = header_height;
                mCloseImage.HorizontalAlignment = HorizontalAlignment.Left;
                mCloseImage.VerticalAlignment = VerticalAlignment.Top;

                mCloseImage.MouseLeftButtonDown += MouseDown;
                mCloseImage.MouseLeftButtonUp += MouseUp;
                mCloseImage.MouseLeave += MouseLeave;
                mCloseImage.MouseEnter += MouseEnter;

                mPanel.Children.Add(mCloseImage);

                mBackImage = new Image();
                mBackImage.Margin = new Thickness(10, 0, 0, 0);
                mBackImage.Source = new BitmapImage(new Uri("Resources/igb_back.png", UriKind.Relative));
                mBackImage.Width = header_height;
                mBackImage.Height = header_height;
                mBackImage.HorizontalAlignment = HorizontalAlignment.Left;
                mBackImage.VerticalAlignment = VerticalAlignment.Bottom;

                mBackImage.MouseLeftButtonDown += MouseDown;
                mBackImage.MouseLeftButtonUp += MouseUp;
                mBackImage.MouseLeave += MouseLeave;
                mBackImage.MouseEnter += MouseEnter;

                mPanel.Children.Add(mBackImage);

                mForwardImage = new Image();
                mForwardImage.Margin = new Thickness(header_height + 40, 0, 0, 0);
                mForwardImage.Source = new BitmapImage(new Uri("Resources/igb_forward.png", UriKind.Relative));
                mForwardImage.Width = header_height;
                mForwardImage.Height = header_height;
                mForwardImage.HorizontalAlignment = HorizontalAlignment.Left;
                mForwardImage.VerticalAlignment = VerticalAlignment.Bottom;

                mForwardImage.MouseLeftButtonDown += MouseDown;
                mForwardImage.MouseLeftButtonUp += MouseUp;
                mForwardImage.MouseLeave += MouseLeave;
                mForwardImage.MouseEnter += MouseEnter;

                mPanel.Children.Add(mForwardImage);

                mReloadImage = new Image();
                mReloadImage.Margin = new Thickness(0, 0, 10, 0);
                mReloadImage.Source = new BitmapImage(new Uri("Resources/igb_refresh.png", UriKind.Relative));
                mReloadImage.Width = header_height;
                mReloadImage.Height = header_height;
                mReloadImage.HorizontalAlignment = HorizontalAlignment.Right;
                mReloadImage.VerticalAlignment = VerticalAlignment.Bottom;

                mReloadImage.MouseLeftButtonDown += MouseDown;
                mReloadImage.MouseLeftButtonUp += MouseUp;
                mReloadImage.MouseLeave += MouseLeave;
                mReloadImage.MouseEnter += MouseEnter;

                mPanel.Children.Add(mReloadImage);

                if (mWebView == null)
                    mWebView = new WebBrowser();
                mWebView.Width = mPanel.ActualWidth;
                mWebView.Height = mPanel.ActualHeight - header_height * 2;
                mWebView.IsScriptEnabled = true;
                if (!inited_navig)
                {
                    mWebView.Navigating += NavigationStarting;
                    mWebView.Navigated += NavigationCompleted;
                    mWebView.NavigationFailed += NavigationFailed;
                    inited_navig = true;
                }
                mPanel.Children.Add(mWebView);

                prog_ring = new ProgressBar();
                prog_ring.Margin = new Thickness(0, header_height, 0, 0);
                prog_ring.Width = mPanel.ActualWidth;
                prog_ring.Height = 5;
                prog_ring.HorizontalAlignment = HorizontalAlignment.Center;
                prog_ring.VerticalAlignment = VerticalAlignment.Top;

                mPanel.Children.Add(prog_ring);

                if (CTG_TYPE == "SUPPORT")
                {
                    string url = INGAMEBROWSER_URL + "?ctg=SUPPORT&" + solveTemplate(INGAMEBROWSER_POST_TEMPLATE);

                    _debug("SUPPORT URL: " + url);

                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    return;
                }
                if (CTG_TYPE.StartsWith("BANNED"))
                {
                    string url = INGAMEBROWSER_URL + "?ctg=BANNED&" + solveTemplate(INGAMEBROWSER_POST_TEMPLATE) + "&extra_14=" + CTG_TYPE.Split(new char[] { ':' })[1];

                    _debug("SUPPORT URL: " + url);

                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    return;
                }
                if (CTG_TYPE == "NEWS")
                {
                    string url = DISPLAY_NEWS + "&" + solveTemplate(INGAMEBROWSER_POST_TEMPLATE);

                    _debug("NEWS URL: " + url);

                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    return;
                }
                if (CTG_TYPE == "FORUM")
                {
                    string url = FORUM_URL + solveTemplate(FORUM_PARAMS_TEMPLATE);

                    _debug("FORUM URL: " + url);

                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    return;
                }
                if (CTG_TYPE.StartsWith("url:"))
                {
                    string url = CTG_TYPE.Replace("url:", "");

                    _debug("IGB URL: " + url);

                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    return;
                }
                
            }
            else
            {
                if (CTG_TYPE == "NEWS")
                    RefreshUnreadNewsNumberInternal();

                mWebView.NavigateToString("");

                mPanel.Children.Remove(mWebView);
                mPanel.Children.Remove(mBackgroundImage);
                mPanel.Children.Remove(mBackImage);
                mPanel.Children.Remove(mForwardImage);
                mPanel.Children.Remove(mReloadImage);
                mPanel.Children.Remove(mCloseImage);
                mPanel.Children.Remove(prog_ring);

                //mWebView = null;
                mBackgroundImage = null;
                mBackImage = null;
                mForwardImage = null;
                mReloadImage = null;
                mCloseImage = null;
                prog_ring = null;

                CTG_TYPE = "";
            }
        }

        private void MouseEnter(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ((Image)sender).Opacity = 0.9;
        }

        private void MouseLeave(object sender, System.Windows.Input.MouseEventArgs e)
        {
            ((Image)sender).Opacity = 1;
        }

        private void MouseUp(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ((Image)sender).Opacity = 1;

            if (sender == mCloseImage)
            {
                Show("", false);
                return;
            }
            if (sender == mBackImage)
            {
                if (mWebView != null && mWebView.CanGoBack)
                    mWebView.GoBack();
                return;
            }
            if (sender == mForwardImage)
            {
                if (mWebView != null && mWebView.CanGoForward)
                    mWebView.GoForward();
                return;
            }
            if (sender == mReloadImage)
            {
                if (mWebView != null)
                    mWebView.Navigate(new Uri(s_URL, UriKind.Absolute));                    
                return;
            }
        }

        private void MouseDown(object sender, System.Windows.Input.MouseButtonEventArgs e)
        {
            ((Image)sender).Opacity = 0.6;
        }

        protected void _debug(string message)
        {
            System.Diagnostics.Debug.WriteLine("[InGameBrowser]: " + message);
        }

        private void NavigationStarting(object sender, NavigatingEventArgs args)
        {
            if (args != null && args.Uri != null)
                _debug("NavigationStarting: " + args.Uri.ToString());
            s_URL = args.Uri.ToString();
            if (prog_ring != null)
                prog_ring.IsIndeterminate = true;
        }

        private void NavigationCompleted(object sender, NavigationEventArgs args)
        {
            if (args != null && args.Uri != null)
                _debug("NavigationCompleted: " + args.Uri.ToString());
            if (prog_ring != null)
                prog_ring.IsIndeterminate = false;
        }

        private void NavigationFailed(object sender, NavigationFailedEventArgs args)
        {
            if (args != null && args.Uri != null)
                _debug("NavigationFailed: " + args.Uri.ToString());
            if (prog_ring != null)
                prog_ring.IsIndeterminate = false;
        }
    }
}
