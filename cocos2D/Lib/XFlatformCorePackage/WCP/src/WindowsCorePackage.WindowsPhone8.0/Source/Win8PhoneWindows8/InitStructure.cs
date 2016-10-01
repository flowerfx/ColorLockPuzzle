using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

#if WINDOWS_APP
using Windows.UI.Xaml.Controls;
#endif

//#if WINDOWS_PHONE
using System.Windows.Controls;
//#endif

namespace WCPToolkitWinPhone80Bridge
{
    namespace InGameBrowserLibrary
    {
        public sealed class InitParams
        {
            public string Language { get; set; }
            public string AnonymousAccount { get; set; }
            public string FacebookId { get; set; }
            public string GLiveAccount { get; set; }
            public string GameCode { get; set; }
            public string GameOperator { get; set; }
            public string GameVersion { get; set; }
            public string HDIDFV { get; set; }
            public string BaseUrl { get; set; }
            public string ClientID { get; set; }
            public string WAID { get; set; }
            public string XboxID { get; set; }
            public int UserAge { get; set; }
            public string OsVersion { get; set; }

            public InitParams()
            {
                Language = null;
                AnonymousAccount = null;
                FacebookId = null;
                GLiveAccount = null;
                GameCode = null;
                GameOperator = null;
                GameVersion = null;
                HDIDFV = null;
                BaseUrl = null;
                ClientID = null;
                WAID = null;
                XboxID = null;
                UserAge = -1;
            }

            internal bool CheckParams()
            {
                bool isValid = true;

                int languageIndex = Array.FindIndex(InGameBrowser.TXT_IGB_LANGUAGES, lan => lan.Equals(Language, StringComparison.Ordinal));
                if (languageIndex == -1)
                {
#if DEBUG
                    
#endif
                    Language = "EN";
                    isValid = false;
                }

                if (AnonymousAccount == null || AnonymousAccount == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid Anonymous Account
#endif
                    AnonymousAccount = String.Empty;
                    isValid = false;
                }

                if (FacebookId == null)
                {   //Not mandatory
                    FacebookId = String.Empty;
                }

                if (GLiveAccount == null)
                {   //Not mandatory
                    GLiveAccount = String.Empty;
                }

                if (GameCode == null || GameCode == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid Game Code
#endif
                    GameCode = String.Empty;
                    isValid = false;
                }

                if (GameOperator == null || GameOperator == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid Game Operator
#endif
                    GameOperator = String.Empty;
                    isValid = false;
                }

                if (HDIDFV == null || HDIDFV == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid HDIDFV
#endif
                    HDIDFV = String.Empty;
                    isValid = false;
                }

                if (BaseUrl == null || BaseUrl == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid Base Url
#endif
                    BaseUrl = String.Empty;
                    isValid = false;
                }

                if (ClientID == null || ClientID == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid Client ID
#endif
                    ClientID = String.Empty;
                    isValid = false;
                }

                if (WAID == null || WAID == String.Empty)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid WAID
#endif
                    WAID = String.Empty;
                    isValid = false;
                }

                if (XboxID == null)
                {   //Not mandatory
                    XboxID = String.Empty;
                }

                if (UserAge <= 0)
                {
#if DEBUG
                    Debugger.Break(); //Please set a valid User Age
#endif
                    UserAge = 0;
                    isValid = false;
                }

                return isValid;
            }

        }
    }
}