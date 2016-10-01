using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Diagnostics;
using System.Windows.Media;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using System.Net;
using System.Threading;
using System.IO.IsolatedStorage;
using Microsoft.Phone.Net.NetworkInformation;
using Microsoft.Devices.Sensors;
using System.Xml.Linq;
using Microsoft.Phone.Tasks;
using System.Windows.Threading;

namespace WCPToolkitWinPhone80
{
    public enum InternetConnectionType
	{
		EthernetConnection = 0,
		WirelessConnection,

		Mobile4GConnection,
		Mobile3GConnection,
		Mobile2GConnection,
		MobileConnection,		//Unknown mobile connection

        LocalAccess,
        ConstrainedIAccess,

		UnknownConnection,
		NoConnection
	};

    public class WCPToolkitWinPhone80ContainerClass
    {

        static WCPToolkitWinPhone80ContainerClass  m_pInstance = null;

        private static int s_width = -1;
        private static int s_height = -1;

        public static WCPToolkitWinPhone80ContainerClass GetInstance()
        {
            if (m_pInstance == null)
            {
                m_pInstance = new WCPToolkitWinPhone80ContainerClass();
            }

            return m_pInstance;
        }

        public string TrueFalseString()
        {
            return AntiHack.AntiHack.GetInstance().GetTestString();
        }
        public string GetAdvertisingID()
        {
            var type = Type.GetType("Windows.System.UserProfile.AdvertisingManager, Windows, Version=255.255.255.255, Culture=neutral, PublicKeyToken=null, ContentType=WindowsRuntime");
            return type != null ? (string)type.GetProperty("AdvertisingId").GetValue(null, null) : "";
        }

        public string GetDeviceName()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceName;
        }
        public string GetDeviceFirmware()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceFirmwareVersion;
        }
        public string GetDeviceManufacturer()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceManufacturer;
        }
        public float GetDeviceTotalMemory()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceTotalMemory / (1000 * 1000.0f);//in MB
        }
        public float GetFreeRam()
        {
            return (Microsoft.Phone.Info.DeviceStatus.DeviceTotalMemory - Microsoft.Phone.Info.DeviceStatus.ApplicationCurrentMemoryUsage) / (1000 * 1000.0f);//in MB
        }
	    public float GetCurrentMemoryUsage()
        {
            return Microsoft.Phone.Info.DeviceStatus.ApplicationCurrentMemoryUsage / 1000000.0f;
        }
	    public float GetPeakMemoryUsage()
        {
            return Microsoft.Phone.Info.DeviceStatus.ApplicationPeakMemoryUsage / 1000000.0f;
        }
        public float GetMemoryWorkingLimit()
        {
            string sValue = Microsoft.Phone.Info.DeviceExtendedProperties.GetValue("ApplicationWorkingSetLimit").ToString();
            float fValue = float.Parse(sValue) / 1000000.0f;
            return fValue;
        }
        public int GetAppMemoryUsageLimit()
        {
            return (int)Microsoft.Phone.Info.DeviceStatus.ApplicationMemoryUsageLimit / 1000000;
        }
        public float GetFreeDiskSpace()
        {
            return IsolatedStorageFile.GetUserStoreForApplication().AvailableFreeSpace / 1024.0f;
        }
        public string GetOSVersion()
        {
            int major = System.Environment.OSVersion.Version.Major;
            int minor = System.Environment.OSVersion.Version.Minor;

            string systemVersion = major.ToString() + "." + minor.ToString();

            return systemVersion;
        }
        public int GetWindowWidth()
        {
            if (s_width == -1)
            {
                double value = 0;
                int scaleFactor = 1;

                value = Math.Floor(System.Windows.Application.Current.Host.Content.ActualWidth * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f);
                scaleFactor = Application.Current.Host.Content.ScaleFactor;

                try
                {
                    //1080p
                    Size size = (Size)Microsoft.Phone.Info.DeviceExtendedProperties.GetValue("PhysicalScreenResolution");
                    value = size.Width;
                }
                catch (Exception)
                {
                    if (scaleFactor == 100)
                    {
                        // WVGA
                        value = 480;
                    }
                    else if (scaleFactor == 160)
                    {
                        // WXGA
                        value = 768;
                    }
                    else if (scaleFactor == 150)
                    {
                        //720p
                        value = 720;
                    }
                }

                s_width = (int)value;
            }

            return s_width;
        }

        public int GetWindowHeight()
        {
            if (s_height == -1)
            {
                double value = 0;
                int scaleFactor = 1;

                if (Deployment.Current.Dispatcher.CheckAccess()
                    || System.ComponentModel.DesignerProperties.IsInDesignTool)
                {
                    value = Math.Floor(System.Windows.Application.Current.Host.Content.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f);
                    scaleFactor = Application.Current.Host.Content.ScaleFactor;
                }
                else
                {
                    System.Threading.EventWaitHandle Wait = new System.Threading.AutoResetEvent(false);
                    Deployment.Current.Dispatcher.BeginInvoke(() =>
                    {
                        value = Math.Floor(System.Windows.Application.Current.Host.Content.ActualHeight * Application.Current.Host.Content.ScaleFactor / 100.0f + 0.5f);
                        scaleFactor = Application.Current.Host.Content.ScaleFactor;
                        Wait.Set();
                    });
                    Wait.WaitOne();
                    Wait.Dispose();
                }

                try
                {
                    //1080p
                    Size size = (Size)Microsoft.Phone.Info.DeviceExtendedProperties.GetValue("PhysicalScreenResolution");
                    value = size.Height;
                }
                catch (Exception)
                {
                    if (scaleFactor == 100)
                    {
                        // WVGA
                        value = 800;
                    }
                    else if (scaleFactor == 160)
                    {
                        // WXGA
                        value = 1280;
                    }
                    else if (scaleFactor == 150)
                    {
                        //720p
                        value = 1280;
                    }
                }


                s_height = (int)value;
            }

            return s_height;
        }

        public string GetDeviceGPUName()
        {
            return Microsoft.Phone.Info.DeviceStatus.DeviceName;

            //[BUC-SO] maybe use this?
    //IDXGIFactory* factory;
    //IDXGIAdapter* adapter;
    //DXGI_ADAPTER_DESC adapterDesc;

    //CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)&factory);
    //factory->EnumAdapters(0, &adapter);
    //adapter->GetDesc(&adapterDesc);

    //char ch[128];
    //char DefChar = ' ';
    //WideCharToMultiByte(CP_ACP, 0, adapterDesc.Description, -1, ch, 128, &DefChar, NULL);

    //std::string ss(ch);

    //return ss;
        }


        public int RemoveFile(string filename)
        {
            try
            {
                IsolatedStorageFile IsolatedStorage = IsolatedStorageFile.GetUserStoreForApplication();
                IsolatedStorage.DeleteFile(filename);
                return 0;
            }
            catch (Exception e)
            {
                System.Diagnostics.Debug.WriteLine("[Win8PhoneContainerClass: RemoveFile]: " + e.Message);
                return -1;
            }
        }
        public int RenameFile(string filename1, string filename2)
        {
            try
            {
                IsolatedStorageFile IsolatedStorage = IsolatedStorageFile.GetUserStoreForApplication();

                if (IsolatedStorage.FileExists(filename2))
                    IsolatedStorage.DeleteFile(filename2);
                if (IsolatedStorage.FileExists(filename1))
                {
                    IsolatedStorage.MoveFile(filename1, filename2);
                    IsolatedStorage.DeleteFile(filename1);
                }
                return 0;
            }
            catch (Exception e)
            {
                System.Diagnostics.Debug.WriteLine("[Win8PhoneContainerClass: RenameFile]: " + e.Message);
                return -1;
            }
        }
        public void SetScreenSleepMode(bool bSleep)
        {
            Microsoft.Phone.Shell.PhoneApplicationService.Current.UserIdleDetectionMode = (bSleep ? Microsoft.Phone.Shell.IdleDetectionMode.Enabled : Microsoft.Phone.Shell.IdleDetectionMode.Disabled);
        }
        public bool IsScreenSleepModeOn()
        {
            return (Microsoft.Phone.Shell.PhoneApplicationService.Current.UserIdleDetectionMode == Microsoft.Phone.Shell.IdleDetectionMode.Enabled);
        }
        public void LaunchBrowser(string url)
        {
            //If you use Windows store apps project (WP8.1/Win8.1), WebBrowserTask class was replaced by Launcher class.
            //Now you must use LaunchUriAsync method from Launcher class
            Microsoft.Phone.Tasks.WebBrowserTask webBrowserTask = new Microsoft.Phone.Tasks.WebBrowserTask();
            webBrowserTask.Uri = new Uri(url, UriKind.Absolute);
            webBrowserTask.Show();
        }
        public void LaunchMarketplaceDetail()
        {
            Microsoft.Phone.Tasks.MarketplaceDetailTask marketplaceDetailTask = new Microsoft.Phone.Tasks.MarketplaceDetailTask();
            marketplaceDetailTask.Show();
        }
        public void LaunchMarketplaceReview()
        {
            Microsoft.Phone.Tasks.MarketplaceReviewTask marketplaceReviewTask = new Microsoft.Phone.Tasks.MarketplaceReviewTask();
            marketplaceReviewTask.Show();
        }
        public string GetDeviceLanguage()
        {
            return System.Globalization.CultureInfo.CurrentUICulture.TwoLetterISOLanguageName;
        }
        public string GetDeviceCarrier()
        {
            return Microsoft.Phone.Net.NetworkInformation.DeviceNetworkInformation.CellularMobileOperator;
        }

        private static InternetConnectionType GetConnectionTypeFromNetworkInterface(NetworkInterfaceInfo info)
        {
            if (info != null)
            {
                if (info.InterfaceState == Microsoft.Phone.Net.NetworkInformation.ConnectState.Disconnected)
                {
                    return InternetConnectionType.NoConnection;
                }

                switch (info.InterfaceSubtype)
                {
                    case NetworkInterfaceSubType.Unknown:
                        return InternetConnectionType.NoConnection;
                    case NetworkInterfaceSubType.Cellular_GPRS:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_1XRTT:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_EVDO:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_EDGE:
                        return InternetConnectionType.Mobile2GConnection;
                    case NetworkInterfaceSubType.Cellular_3G:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_HSPA:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Cellular_EVDV:
                        return InternetConnectionType.Mobile3GConnection;
                    case NetworkInterfaceSubType.Desktop_PassThru:
                        return InternetConnectionType.UnknownConnection; // for now it's unknown
                    case NetworkInterfaceSubType.WiFi:
                        return InternetConnectionType.WirelessConnection;
                    case NetworkInterfaceSubType.Cellular_LTE:
                        return InternetConnectionType.Mobile4GConnection;
                    case NetworkInterfaceSubType.Cellular_EHRPD:
                        return InternetConnectionType.Mobile4GConnection;
                    default:
                        return InternetConnectionType.UnknownConnection;
                }
            }
            return InternetConnectionType.NoConnection;
        }

        public int GetInternetConnectionInfo()
        {
            NetworkInterfaceList list = new NetworkInterfaceList();
            InternetConnectionType connectionType;

            connectionType = InternetConnectionType.NoConnection;

            foreach (NetworkInterfaceInfo info in list)
            {
                InternetConnectionType res = GetConnectionTypeFromNetworkInterface(info);
                if (res != InternetConnectionType.NoConnection && res < connectionType)
                {
                    connectionType = res;
                }
            }

            return (int)connectionType;
        }

        public bool TrueFalseBool()
        {
            try
            {
                if (System.Diagnostics.Debugger.IsAttached == true) //then WMAppPRHeader.xml file will be added during AppHub certification only! So this has to be skipped during development.
                    return false;

                //scramble WMAppPRHeader.xml file name to make life a little harder in case of reverse engineering
                string fl = "xxx" + "W" + "xxxx" + "M" + "xxxx" + "A" + "xxxx" + "p" + "xxxpxxx" + "PxR" + "xxxxx" + "Hxxxxxxx" + "exxxxxxa" + "xxxx" + "d" + "xxxx" + "xxxxe" + "rxx" + "xxx";
                fl = fl.Replace("x", string.Empty) + "." + "x" + "m" + "l";
                XDocument doc = XDocument.Load(fl); //is hacked, this file is missing or empty!!!
                return false;
            }
            catch (Exception)
            {
                MessageBox.Show("This app was pirated and is not safe to use, please download the original one from Marketplace.");
                MarketplaceDetailTask marketplaceDetailTask = new MarketplaceDetailTask();

                //ProcutdID will be changed after APpHub certification, so has to be read from manifest!

                marketplaceDetailTask.ContentIdentifier = GetProductId().Replace("{", string.Empty).Replace("}", string.Empty).Trim();
                marketplaceDetailTask.ContentType = MarketplaceContentType.Applications;
                marketplaceDetailTask.Show();

                return true;
            }
        }
        public string GetProductId()
        {

            System.Xml.Linq.XElement xml = System.Xml.Linq.XElement.Load("WMAppManifest.xml");

            var appElement = (from manifest in xml.Descendants("App") select manifest).SingleOrDefault();

            if (appElement != null)
            {

                return appElement.Attribute("ProductID").Value;

            }

            return string.Empty;

        }

       
        public void AddLiveTile( string pictureName)
        {
			try
			{
				LiveTiles.LiveTile.GetInstance().AddLiveTile(pictureName);
			}
			catch (Exception e)
			{
				System.Diagnostics.Debug.WriteLine("[Win8PhoneContainerClass: AddWidgetMessage]: " + e.Message);
			}

        }
        public void UpdateLiveTile(bool title)
        {
			LiveTiles.LiveTile.GetInstance().UpdateLiveTile(title);

        }

		public void RemoveLiveTiles()
		{
			LiveTiles.LiveTile.GetInstance().RemoveLiveTiles();

		}
       

        public void ExitApplication()
        {
            Application.Current.Terminate();
        }


		public int ShowToastNotification(string title, string content, int delay)
		{
            return LocalToastNotifications.LocalToastNotification.GetInstance().ShowToastNotification(title, content, delay);
		}

		public void RemoveToastNotificationsById(int id)
		{
            LocalToastNotifications.LocalToastNotification.GetInstance().RemoveToastNotificationsById(id);

		}

    }
}
