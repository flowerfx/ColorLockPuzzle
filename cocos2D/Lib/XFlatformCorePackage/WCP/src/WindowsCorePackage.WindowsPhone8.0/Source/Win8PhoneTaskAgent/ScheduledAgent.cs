using System.Diagnostics;
using System.Windows;
using Microsoft.Phone.Scheduler;
using System;
using System.Collections.Generic;
using Microsoft.Phone.Shell;

namespace WCPToolkitWinPhone80TaskAgent
{
    public class ScheduledAgent : ScheduledTaskAgent
    {
        /// <remarks>
        /// ScheduledAgent constructor, initializes the UnhandledException handler
        /// </remarks>
        static ScheduledAgent()
        {
            // Subscribe to the managed exception handler
            Deployment.Current.Dispatcher.BeginInvoke(delegate
            {
                Application.Current.UnhandledException += UnhandledException;
            });
        }

        /// Code to execute on Unhandled Exceptions
        private static void UnhandledException(object sender, ApplicationUnhandledExceptionEventArgs e)
        {
            if (Debugger.IsAttached)
            {
                // An unhandled exception has occurred; break into the debugger
                Debugger.Break();
            }
        }

        /// <summary>
        /// Agent that runs a scheduled task
        /// </summary>
        /// <param name="task">
        /// The invoked task
        /// </param>
        /// <remarks>
        /// This method is called when a periodic or resource intensive task is invoked
        /// </remarks>
        protected override void OnInvoke(ScheduledTask task)
        {
            List<LocalToastNotifications.Toast> localToasts = new List<LocalToastNotifications.Toast>();


            localToasts = LocalToastNotifications.LocalToastNotification.GetInstance().LoadFromDisk();


            for (var i = 0; i < localToasts.Count; i++)
            {

                DateTime dt = Convert.ToDateTime(localToasts[i].date_to_show);

                if (DateTime.Compare(dt, (DateTime.Now)) < 0)
                {
                    ShellToast toast = new ShellToast();
                    toast.Title = localToasts[i].title;
                    toast.Content = localToasts[i].content;
                    toast.Show();

                    localToasts.Remove(localToasts[i]);

                }

            }

            LocalToastNotifications.LocalToastNotification.GetInstance().SaveToDisk(localToasts);

            NotifyComplete();
        }
    }
}