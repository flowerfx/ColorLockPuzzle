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

using WCPToolkit.Utils.Encryption;
using WCPToolkit.Utils.ZOrdering;

namespace WCPToolkitWinPhone80Bridge
{
    namespace InGameBrowserLibrary
    {
        public delegate void UnreadNewsChangedCallback(int unreadNewsNumber);
        public delegate void DisplayStateChangedDelegete();
        //public delegate void InGameRedirectCallback(string arg);

        public enum LinkCategory
        {
            NONE,
            URL,
            SUPPORT,
            BANNED,
            FORUM,
            TERMS,
            PRIVACY,
            NEWS,
            EULA,
            FACEBOOKSHARE,
            FACEBOOKFANPAGE,
            TWITTER,
            GAMEUPDATE,
            GAMEREVIEW
        }

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

            private string INGAMEBROWSER_URL = "";
            private string INGAMEBROWSER_POST_TEMPLATE = "from=FROM&op=OPERATOR&country=COUNTRY&lg=LANG&udid=UDID&game_ver=VERSION&d=DEVICE&f=FIRMWARE&anonymous=ANONYMOUS_ACCOUNT&fbid=FACEBOOK_ID&gliveusername=GLIVE_USERNAME&xbox=XBOX_ID&os=winp&hdidfv=HDIDFV&clientid=CLIENT_ID&user_age=CURRENT_USER_AGE";

            private string REDIRECT_URL = "";
            private string REDIRECT_PARAMS_TEMPLATE = "from=FROM&op=OPERATOR&game_ver=VERSION&lg=LANG&country=COUNTRY&d=DEVICE&f=FIRMWARE&udid=UDID&hdidfv=HDIDFV&clientid=CLIENT_ID&user_age=CURRENT_USER_AGE";

            private string FACEBOOK_PARAMS_TEMPLATE = "from=FROM&op=OPERATOR&game_ver=VERSION&lg=LANG&country=COUNTRY&d=DEVICE&f=FIRMWARE&udid=UDID&hdidfv=HDIDFV";

            private string CHECK_UNREAD_NEWS_NUMBER = "";
            private string SAVE_NEWS_ID = "";
            private string DISPLAY_NEWS = "";

            private int languageIndex = 0;
            private string anonymousAccount = "";
            private string facebookID = "";
            private string gliveAccount = "";
            private string gameCode = "";
            private string gameOperator = "";
            private string gameVersion = "";

            private string HDIDFV = "";
            private LinkCategory CTG_TYPE = LinkCategory.NONE;
            private string clientID = "";
            private string waid = ""; //NOT USED CURRENTLY
            private string XboxID = "";
            private int userAge = 0;
            private string m_firmware = "";

            private enum UserGender
            {
                GENDER_UNKNOWN,
                GENDER_MALE,
                GENDER_FEMALE,
            };
            private UserGender userGender = UserGender.GENDER_UNKNOWN;

            private bool m_IGB_initialized = false;
            private bool m_isShowing = false;
			
            private static int unreadNewsNumber = -1;

            private System.Windows.Threading.Dispatcher s_dispatcher = null;

            private UnreadNewsChangedCallback unreadCallback = null;
            //private InGameRedirectCallback redirectCallback = null;

            internal static string[] TXT_IGB_LANGUAGES = { "EN", "FR", "DE", "IT", "SP", "JP", "KR", "CN", "BR", "RU", "TR", "AR", "TH", "ID", "VI", "ZT" };

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

            public event DisplayStateChangedDelegete Shown;
            public event DisplayStateChangedDelegete Hidden;

            private string solveTemplate(string template)
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
                            .Replace("FIRMWARE", m_firmware)
                            .Replace("ANONYMOUS_ACCOUNT", anonymousAccount)
                            .Replace("FACEBOOK_ID", facebookID)
                            .Replace("GLIVE_USERNAME", gliveAccount)
                            .Replace("XBOX_ID", XboxID)
                            .Replace("HDIDFV", HDIDFV)
                            .Replace("CLIENT_ID", clientID)
                            .Replace("CURRENT_USER_AGE", userAge.ToString())
                            .Replace(" ", "");
                }
                else return "";

                return result;
            }
	        public void Init(InitParams initParams)
	        {
	            m_IGB_initialized = initParams.CheckParams();
	            if (m_IGB_initialized)
	            {
                    SetGameLanguage(initParams.Language);
	                SetAnonymousAccount(initParams.AnonymousAccount);
	                SetFacebookId(initParams.FacebookId);
	                SetGLiveAccount(initParams.GLiveAccount);
	                SetGameCode(initParams.GameCode);
	                SetGameOperator(initParams.GameOperator);
	                SetGameVersion(initParams.GameVersion);
	                SetHDIDFV(initParams.HDIDFV);
	                SetBaseUrl(initParams.BaseUrl);
	                SetClientID(initParams.ClientID);
                    SetWaid(initParams.WAID);
                    SetXboxID(initParams.XboxID);
                    SetUserAge(initParams.UserAge);
                    m_firmware = initParams.OsVersion;
	            }
	        }

            private string getUDID()
            {
                return Encryptor.Encrypt_MD5(Windows.Phone.System.Analytics.HostInformation.PublisherHostId);
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

            public void SetGameLanguage(string language)
            {
                int index = Array.FindIndex(TXT_IGB_LANGUAGES, lan => lan.Equals(language, StringComparison.Ordinal));
                if (index != -1)
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

            public void SetWaid(string sWaid)
            {
                waid = sWaid;
            }

            public void SetXboxID(string sXboxID)
            {
                XboxID = sXboxID;
            }

            public void SetBaseUrl(string url)
            {
                if (url != null && url != String.Empty)
                {
                    if (!url.EndsWith("/"))
                        url += "/";

                    INGAMEBROWSER_URL = url + "redir/ingamebrowser.php";
                    REDIRECT_URL = url + "redir/";
                    CHECK_UNREAD_NEWS_NUMBER = url + "redir/ingamenews.php?action=checkNews&last-id=LAST_ID";
                    SAVE_NEWS_ID = url + "redir/ingamenews.php?action=saveNews&last-id=LAST_ID";
                    DISPLAY_NEWS = url + "redir/ingamenews.php?action=displayNews";
                }
            }
		
	        public void SetClientID(string id)
	        {
	            if (id != null)
	                clientID = id;
	        }

            public void SetUserAge(int age)
            {
                if (age < 13)
                    userAge = 0; //FOR COPPA AND OFT: DON'T SEND REAL USER AGE IF HE IS UNDER 13
                else
                    userAge = age;
            }

            public void SetUserGender(int gender)
            {
                userGender = (UserGender)gender;
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
                
                s_dispatcher = parent.Dispatcher;
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
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.SUPPORT);
            }

            public void ShowCustomerCareWithBanType(int ban_type)
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.BANNED, ban_type.ToString());
            }

            public void ShowInGameBrowserWithUrl(string url)
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.URL, url);
            }

            public void ShowForum()
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.FORUM);
            }

            public void ShowTerms()
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.TERMS);
            }

            public void ShowNews()
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.NEWS);
            }

            public void ShowPrivacyPolicy()
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.PRIVACY);
            }

            public void ShowEULA()
            {
                if (!m_IGB_initialized)
                    return;
                CallShowOnUI(LinkCategory.EULA);
            }

            public void ShowFaceBookShare(bool openExternally)
            {
                if (!m_IGB_initialized)
                    return;
                if (openExternally)
                    OpenInExternalBrowser(LinkCategory.FACEBOOKSHARE);
                else
                    CallShowOnUI(LinkCategory.FACEBOOKSHARE);
            }

            public void ShowFaceBookPage(bool openExternally)
            {
                if (!m_IGB_initialized)
                    return;
                if (openExternally)
                    OpenInExternalBrowser(LinkCategory.FACEBOOKFANPAGE);
                else
                    CallShowOnUI(LinkCategory.FACEBOOKFANPAGE);
            }

            private void CallShowOnUI(LinkCategory cat, String url = "")
            {
                try
                {
                    if (s_dispatcher == null)
                        s_dispatcher = Deployment.Current.Dispatcher;
                    s_dispatcher.BeginInvoke(
                        delegate
                        {
                            Show();
                            NavigateTo(cat, url);
                        });
                }
                catch (Exception e)
                {
                    _debug(" [CallShowOnUI] exception: " + e.Message);
                }
            }

            private void OpenInExternalBrowser(LinkCategory cat, String extra = "")
            {
                var uri = new Uri(GetLink(cat, extra));
                Deployment.Current.Dispatcher.BeginInvoke(
                delegate
                {
#pragma warning disable 4014
                    Windows.System.Launcher.LaunchUriAsync(uri);
#pragma warning restore 4014
                });
            }

            private void addGender(ref string parameters)
            {
                switch (userGender)
                {
                    case UserGender.GENDER_MALE:
                        parameters += "&gender=male";
                        break;
                    case UserGender.GENDER_FEMALE:
                        parameters += "&gender=female";
                        break;
                }
            }

            public String GetLink(LinkCategory cat, string extraParam)
            {
                string url = String.Empty, toEncript = string.Empty;

                switch (cat)
                {
                    case LinkCategory.SUPPORT:
                        url = INGAMEBROWSER_URL + "?ctg=SUPPORT";
                        toEncript = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.BANNED:
                        url = INGAMEBROWSER_URL + "?ctg=BANNED";
                        toEncript = String.Format("extra_14={0}&", extraParam) + solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.FORUM:
                        url = REDIRECT_URL + "?ctg=FORUM";
                        toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.TERMS:
                        url = REDIRECT_URL + "?ctg=TERMS";
                        toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.PRIVACY:
                        url = REDIRECT_URL + "?ctg=PRIVACY";
                        toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.NEWS:
                        url = DISPLAY_NEWS;
                        toEncript = solveTemplate(INGAMEBROWSER_POST_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.EULA:
                        url = REDIRECT_URL + "?ctg=EULA";
                        toEncript = solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.FACEBOOKSHARE:
                        url = REDIRECT_URL + "?ctg=FBOOK";
                        toEncript = String.Format("game={0}&", gameCode) + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
                        break;
                    case LinkCategory.FACEBOOKFANPAGE:
                        url = REDIRECT_URL + "?t=facebook";
                        toEncript = String.Format("game={0}&", gameCode) + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
                        break;
                    case LinkCategory.TWITTER:
                        url = REDIRECT_URL + "?t=twitter";
                        toEncript = String.Format("game={0}&", gameCode) + solveTemplate(FACEBOOK_PARAMS_TEMPLATE);
                        break;
                    case LinkCategory.GAMEUPDATE:
                        url = REDIRECT_URL + "?ctg=UPDATE";
                        toEncript = String.Format("game={0}&", gameCode) + solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    case LinkCategory.GAMEREVIEW:
                        url = REDIRECT_URL + "?ctg=GAME_REVIEW";
                        toEncript = String.Format("game={0}&", gameCode) + solveTemplate(REDIRECT_PARAMS_TEMPLATE);
                        addGender(ref toEncript);
                        break;
                    default:
                        url = extraParam;
                        break;
                }

                if (!String.IsNullOrEmpty(toEncript))
                {
                    url = url + String.Format("&data={0}&enc=4", Encryptor.Encrypt(toEncript, EncryptionType.EET_AES));
                }

                return url;
            }

            public bool IsInitialized()
            {
                return m_IGB_initialized;
            }

            public bool HandleBackKey()
            {
                if (mWebView != null && m_isShowing)
                {
                    _debug("escape key triggered");
                    if (CTG_TYPE == LinkCategory.NEWS)
                    {
#pragma warning disable 4014
                        RefreshUnreadNewsNumberInternal();
#pragma warning restore 4014
                    }
                    Hide();
                    return true;
                }
                return false;
            }

            private void injectJavaScript(String js)
            {
                mWebView.Dispatcher.BeginInvoke(
                    () =>
                    {
                        mWebView.InvokeScript("eval", new string[] { js });
                    });
            }

            private void RunActionOnUI(Action a)
            {
                try
                {
                    if (s_dispatcher == null)
                        s_dispatcher = Deployment.Current.Dispatcher;
                    s_dispatcher.BeginInvoke(a);
                }
                catch (Exception e)
                {
                    _debug(" [RunActionOnUI] exception: " + e.Message);
                }
            }

            public void OnControllerEvent(int crt_event, double value)
            {
                switch ((WCPToolkit.Utils.Controller.ControllerEvent)crt_event)
                {
                    case WCPToolkit.Utils.Controller.ControllerEvent.UNDEFINED:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.LeftTrigger:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.RightTrigger:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.LeftStickX:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.LeftStickY:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.RightStickX:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.RightStickY:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.DpadEventUp:
                        if (m_isShowing && (int)value == 1)
                            injectJavaScript(String.Format("window.scrollBy(0, {0})", -100));
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.DpadEventDown:
                        if (m_isShowing && (int)value == 1)
                            injectJavaScript(String.Format("window.scrollBy(0, {0})", 100));
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.DpadEventLeft:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.DpadEventRight:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.LeftBumper:
                        if (m_isShowing)
                            RunActionOnUI(() =>
                            {
                                if(mWebView != null && mWebView.CanGoBack)
                                    mWebView.GoBack();
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.RightBumper:
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonY:
                        if (m_isShowing)
                            RunActionOnUI(() =>
                            {
                                if(mWebView != null && mWebView.CanGoForward)
                                    mWebView.GoForward();
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonA:
                        if (m_isShowing)
                            RunActionOnUI(() =>
                            {
                                if (mWebView != null && s_URL!= null)
                                    mWebView.Navigate(new Uri(s_URL, UriKind.Absolute));
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonX:
                        if (m_isShowing)
                            RunActionOnUI(() =>
                            {
                                if(mWebView != null && mWebView.CanGoBack)
                                    mWebView.GoBack();
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonB:
                        if (value == 0)
                            RunActionOnUI(() =>
                            {
                                HandleBackKey();
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonStart:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonSelect:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.ButtonBack:
                        if (value == 0)
                            RunActionOnUI(() =>
                            {
                                HandleBackKey();
                            });
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.LeftStickButton:
                        break;
                    case WCPToolkit.Utils.Controller.ControllerEvent.RightStickButton:
                        break;
                    default:
                        break;
                }
            }

            public bool IsOnScreen()
            {
                return m_isShowing;
            }

            public void RefreshUnreadNewsNumber()
            {
#pragma warning disable 4014
                RefreshUnreadNewsNumberHelper();
#pragma warning restore 4014
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
                catch (JsonReaderException) { }
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
                catch (JsonReaderException e)
                {
                    System.Diagnostics.Debug.WriteLine("[InGameBrowser: RefreshUnreadNewsNumberInternal]: " + e.Message);
                }
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

            private void Show()
            {
                if (m_isShowing)
                    return;
                m_isShowing = true;

                double header_height = (0.1 * mPanel.ActualHeight < 50) ? mPanel.ActualHeight * 0.1 : 50;

                mBackgroundImage = new Image();
                ZOrder.SetZPriority(mBackgroundImage, ZPriority.INTERSTITIAL_HIGH);
                mBackgroundImage.Source = new BitmapImage(new Uri("Resources/igb_background.png", UriKind.Relative));
                mBackgroundImage.Width = mPanel.ActualWidth;
                mBackgroundImage.Height = mPanel.ActualHeight;
                mBackgroundImage.Stretch = Stretch.Fill;

                mPanel.Children.Add(mBackgroundImage);

                mCloseImage = new Image();
                ZOrder.SetZPriority(mCloseImage, ZPriority.INTERSTITIAL_HIGH);
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
                ZOrder.SetZPriority(mBackImage, ZPriority.INTERSTITIAL_HIGH);
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
                ZOrder.SetZPriority(mForwardImage, ZPriority.INTERSTITIAL_HIGH);
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
                ZOrder.SetZPriority(mReloadImage, ZPriority.INTERSTITIAL_HIGH);
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
                {
                    mWebView = new WebBrowser();
                    ZOrder.SetZPriority(mWebView, ZPriority.INTERSTITIAL_HIGH);
                }
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
                ZOrder.SetZPriority(prog_ring, ZPriority.INTERSTITIAL_HIGH);
                prog_ring.Margin = new Thickness(0, header_height, 0, 0);
                prog_ring.Width = mPanel.ActualWidth;
                prog_ring.Height = 5;
                prog_ring.HorizontalAlignment = HorizontalAlignment.Center;
                prog_ring.VerticalAlignment = VerticalAlignment.Top;

                mPanel.Children.Add(prog_ring);

                if (Shown != null)
                    Shown();
            }

            private void Hide()
            {
                m_isShowing = false;
                CTG_TYPE = LinkCategory.NONE;

                mWebView.NavigateToString("");

                mPanel.Children.Remove(mWebView);
                mPanel.Children.Remove(mBackgroundImage);
                mPanel.Children.Remove(mBackImage);
                mPanel.Children.Remove(mForwardImage);
                mPanel.Children.Remove(mReloadImage);
                mPanel.Children.Remove(mCloseImage);
                mPanel.Children.Remove(prog_ring);

                mBackgroundImage = null;
                mBackImage = null;
                mForwardImage = null;
                mReloadImage = null;
                mCloseImage = null;
                prog_ring = null;

                if (Hidden != null)
                    Hidden();
            }

            private void NavigateTo(LinkCategory ctg, String extraParam = "")
            {
                try
                {
                    CTG_TYPE = ctg;
                    string url = GetLink(ctg, extraParam);
                    mWebView.Navigate(new Uri(url, UriKind.Absolute));
                    _debug(String.Format("Navigating to [{0}] {1}", ctg, url));
                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine("[InGameBrowser: NavigateTo]: " + e.Message);
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
                    if (CTG_TYPE == LinkCategory.NEWS)
                    {
#pragma warning disable 4014
                        RefreshUnreadNewsNumberInternal();
#pragma warning restore 4014
                    }

                    Hide();
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

            private void _debug(string message)
            {
                System.Diagnostics.Debug.WriteLine("[InGameBrowser]: " + message);
            }

            private void NavigationStarting(object sender, NavigatingEventArgs args)
            {
                if (args != null && args.Uri != null)
                    _debug("Navigation Started: " + args.Uri.ToString());

                s_URL = args.Uri.ToString();
                if (prog_ring != null)
                    prog_ring.IsIndeterminate = true;
            }

            private void NavigationCompleted(object sender, NavigationEventArgs args)
            {
                if (args != null && args.Uri != null)
                    _debug("Navigation Completed: " + args.Uri.ToString());

                if (prog_ring != null)
                    prog_ring.IsIndeterminate = false;
            }

            private void NavigationFailed(object sender, NavigationFailedEventArgs args)
            {
                if (args != null && args.Uri != null)
                    _debug("Navigation Failed: " + args.Uri.ToString());

                if (prog_ring != null)
                    prog_ring.IsIndeterminate = false;
            }
        }
    }

}
