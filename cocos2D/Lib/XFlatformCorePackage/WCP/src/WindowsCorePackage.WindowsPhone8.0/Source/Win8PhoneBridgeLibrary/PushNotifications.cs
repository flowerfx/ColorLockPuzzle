using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Collections.Generic;
using System.Threading;
using Microsoft.Phone.Notification;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Scheduler;
using System.Threading.Tasks;
using Microsoft.Xna.Framework.Media;

namespace WCPToolkitWinPhone80Bridge
{
    namespace PushNotificationsLibrary
    {
        public sealed class PushNotifications
        {
            private static HttpNotificationChannel                              sPushChannel;
            private static string                                               sEndpoint;
            private static string                                               sAuthentificatedServerName;
            private static WCPToolkit.CallbackPackerPN.CallbackPacker           sCallbackPacker;
            private static EventWaitHandle                                      sWaitHandle;

            static PushNotifications()
            {
                sWaitHandle                = new ManualResetEvent(true);
                sEndpoint                  = "";
                sAuthentificatedServerName = "*.gameloft.com";
            }

            // Implementation Functions For Windows Phone Push Notifications
            // PUBLIC /////////////////////////////////////////////////////////////////////////////
            public static bool      Init()
            {
                try
                {
                    sPushChannel = HttpNotificationChannel.Find("PushNotifications");
                    if (sPushChannel == null)
                    {
                        sWaitHandle.Reset();
                        sPushChannel = new HttpNotificationChannel("PushNotifications", sAuthentificatedServerName);

                        //register the callbacks
                        sPushChannel.ChannelUriUpdated                  += new EventHandler<NotificationChannelUriEventArgs>(ChannelUriUpdated);
                        //s_PushChannel.ConnectionStatusChanged         += new EventHandler<NotificationChannelConnectionEventArgs>(PushChannel_ConnectionStatusChanged);
                        sPushChannel.ErrorOccurred                      += new EventHandler<NotificationChannelErrorEventArgs>(ErrorOccurred);
                        // this is called only when the application is running
                        sPushChannel.ShellToastNotificationReceived     += new EventHandler<NotificationEventArgs>(OnPushNotificationReceived);

                        sPushChannel.Open();
                        sPushChannel.BindToShellTile();
                        sPushChannel.BindToShellToast();

                        return true;
                    }
                    else
                    {
                        sPushChannel.ChannelUriUpdated                  += new EventHandler<NotificationChannelUriEventArgs>(ChannelUriUpdated);
                        //s_PushChannel.ConnectionStatusChanged         += new EventHandler<NotificationChannelConnectionEventArgs>(PushChannel_ConnectionStatusChanged);
                        sPushChannel.ErrorOccurred                      += new EventHandler<NotificationChannelErrorEventArgs>(ErrorOccurred);
                        sPushChannel.ShellToastNotificationReceived     += new EventHandler<NotificationEventArgs>(OnPushNotificationReceived);
                        if (sPushChannel.ChannelUri != null)
                        {
                            sEndpoint = sPushChannel.ChannelUri.ToString();
                            System.Diagnostics.Debug.WriteLine("Push Notifications Endpoint:");
                            System.Diagnostics.Debug.WriteLine("\t" + sEndpoint);
                        }
                        sWaitHandle.Set();
                        return true;
                    }
                }
                catch (Exception e)
                {
                    string s = e.Message;
                    return false;
                }
            }

            public static string    GetEndpoint()
            {
                TimeSpan ts = new TimeSpan(0, 0, 15);
                sWaitHandle.WaitOne(ts);                //wait to get an URI if we just opened a push channel
                return sEndpoint;
            }

            public static void      RegisterCallback(WCPToolkit.CallbackPackerPN.CallbackPacker packer)
            {
                // callback passed was checked in OnlinePushNotifications::RegisterReceivedCallback, packer is valid
                sCallbackPacker = packer;
                System.Diagnostics.Debug.WriteLine("The Push Notification Received Callback has been Registered");
            }

            public static void      UnregisterCallback()
            {
                sCallbackPacker = null;
                System.Diagnostics.Debug.WriteLine("The Push Notification Received Callback has been Unregistered");
            }

            public static void      CloseChannel()
            {
                if (sPushChannel != null)
                {
                    try
                    {
                        sPushChannel.Close();
                        sPushChannel = null;
                        sEndpoint = "";         // clear the Enpoint. can be used as Init() check
                    }
                    catch (InvalidOperationException ex)
                    {
                        System.Diagnostics.Debug.WriteLine(ex.Message);
                    }
                }
            }

            // Helper Functions For Windows Phone Push Notifications
            // PRIVATE ////////////////////////////////////////////////////////////////////////////                       
            private static void     OnPushNotificationReceived(object sender, NotificationEventArgs e)
            {
                System.Diagnostics.Debug.WriteLine("\nOnline PN Received @ " + DateTime.Now + " :");
                foreach (string key in e.Collection.Keys)
                {
                    string auxData;
                    e.Collection.TryGetValue(key, out auxData);
                    System.Diagnostics.Debug.WriteLine("\t" + key.ToString() + "  " + auxData);
                }

                // Get the data from the OnlinePN
                string text1;
                e.Collection.TryGetValue("wp:Text1", out text1);
                if (text1 == null) text1 = "";

                string text2;
                e.Collection.TryGetValue("wp:Text2", out text2);
                if (text2 == null) text2 = "";

                string text3;                                       // will always return null, third string not supported on WindowsPhone
                e.Collection.TryGetValue("wp:Text3", out text3);    // https://msdn.microsoft.com/en-us/library/windows/apps/windows.ui.notifications.toasttemplatetype
                if (text3 == null) text3 = "";                      // kept for consistency with Windows Desktop

                string sound;
                e.Collection.TryGetValue("wp:Sound", out sound);
                if (sound == null) sound = "";

                string launchParamameter;
                e.Collection.TryGetValue("wp:Param", out launchParamameter);
                if (launchParamameter == null) launchParamameter = "";


                // check if callback is set
                if (sCallbackPacker != null)
                {
                    WCPToolkit.CallbackPackerPN.RuntimeOnlinePNContext contextPN = new WCPToolkit.CallbackPackerPN.RuntimeOnlinePNContext();
                    contextPN.text1 = text1;
                    contextPN.text2 = text2;
                    contextPN.text3 = text3;
                    contextPN.launchParameter = launchParamameter;
                    sCallbackPacker.pushNotificationContext = contextPN;

                    System.Diagnostics.Debug.WriteLine("Request to Trigger the Callback sent");
                    bool wasHandled = sCallbackPacker.CallNativeCallback();

                    if (!wasHandled)
                    {
                        // show MESSAGE_BOX as it was not handled
                        ShowMessageBox(text1, text2, sound);
                    }
                }
                else
                {
                    System.Diagnostics.Debug.WriteLine("Callback has not been set, Push Notification Data will not be passed to the game");
                    // callback not set, always show MessageBox
                    ShowMessageBox(text1, text2, sound);
                }
            }

            private static void     ShowMessageBox(string text1, string text2, string sound)
            {
                Deployment.Current.Dispatcher.BeginInvoke(
                () =>
                {
                    // handle sound
                    if (!string.IsNullOrEmpty(sound))
                    {
                        try
                        {
                            Uri uri = new Uri("\\Assets\\PushNotificationsSound\\" + sound, UriKind.RelativeOrAbsolute);
                            Song messageBoxSound = Song.FromUri("messageBoxShow", uri);
                            Microsoft.Xna.Framework.FrameworkDispatcher.Update();
                            MediaPlayer.Volume = 1;
                            MediaPlayer.Stop();
                            MediaPlayer.Play(messageBoxSound);
                        }
                        catch (Exception ex)
                        {
                            System.Diagnostics.Debug.WriteLine("Exception playing sound " + sound + ": " + ex.Message);
                        }
                    }

                    // show message box
                    if (string.IsNullOrEmpty(text2))
                    {
                        MessageBox.Show(text1); // only text1 is present, show MessageBox with it as content
                    }
                    else
                    {
                        MessageBox.Show(text2, text1, MessageBoxButton.OK); // both texts present
                    }
                });
            }

            private static void     ChannelUriUpdated(object sender, NotificationChannelUriEventArgs e)
            {
                sEndpoint = e.ChannelUri.ToString();
                System.Diagnostics.Debug.WriteLine("Push Notifications Endpoint:");
                System.Diagnostics.Debug.WriteLine("\t" + sEndpoint);
                sWaitHandle.Set();
            }

            private static void     ErrorOccurred(object sender, NotificationChannelErrorEventArgs e)
            {
                System.Diagnostics.Debug.WriteLine(String.Format("A push notification {0} error occurred.  {1} ({2}) {3}", e.ErrorType, e.Message, e.ErrorCode, e.ErrorAdditionalData));

                sWaitHandle.Set();
                
                // need to inform the game of the updated Endpoint in order to re-register it with Hermes
                if (sCallbackPacker != null)
                {
                    WCPToolkit.CallbackPackerPN.RuntimeOnlinePNContext contextPN = new WCPToolkit.CallbackPackerPN.RuntimeOnlinePNContext();
                    contextPN.text1             = "Invalid Push Notifications Channel! Check Internet Connection! Recall Init()!";
                    contextPN.text2             = "Details: "       + e.ErrorType;
                    contextPN.text3             = "e.Message"       + e.Message;
                    contextPN.launchParameter   = "ERROR: "         + e.ErrorCode;
                    sCallbackPacker.pushNotificationContext = contextPN;
                    sCallbackPacker.CallNativeCallback();
                }
            }
            

        }
    }

}
