using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.IO; 
using System.IO.IsolatedStorage;
using Microsoft.Phone.Scheduler;

    namespace LocalToastNotifications
    {

        public class Toast
        {

            public int id;
            public string date_to_show;
            public string title;
            public string content;

            public Toast()
            { }
        }


        public class LocalToastNotification
        {
            private const string m_ScheduledTaskName = "WCPLocalToasts";
            private const string m_file = "Toasts.dat";
            public int m_lastId = 0;
            static LocalToastNotification m_pInstance = null;

            public static LocalToastNotification GetInstance()
            {
                if (m_pInstance == null)
                {
                    m_pInstance = new LocalToastNotification();
                }

                return m_pInstance;
            }

            public int ShowToastNotification(string title, string content, int delay)
            {
                List<Toast> localToasts = LoadFromDisk();


                if (localToasts.Count > 0)
                    m_lastId = localToasts[localToasts.Count - 1].id;

                m_lastId++;


                TimeSpan ts = new TimeSpan(0, 0, delay);
                DateTime dt = DateTime.Now + ts;

                Toast t = new Toast();

                t.id = m_lastId;
                t.title = title;
                t.content = content;
                t.date_to_show = dt.ToString();

                localToasts.Add(t);

                SaveToDisk(localToasts);

                PeriodicTask periodicTask = ScheduledActionService.Find(m_ScheduledTaskName) as PeriodicTask;

                if (periodicTask == null)
                {
                    PeriodicTask task = new PeriodicTask(m_ScheduledTaskName);
                    task.Description = "local toasts";

                    ScheduledActionService.Add(task);

                }
#if DEBUG
                ScheduledActionService.LaunchForTest(m_ScheduledTaskName, TimeSpan.FromSeconds(60));
#endif

                return m_lastId;
            }


            public List<Toast> LoadFromDisk()
            {
                IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();

                List<Toast> toasts = new List<Toast>();

                if (storage.FileExists(m_file))
                {
                    IsolatedStorageFileStream stream = null;
                    try
                    {
                        stream = storage.OpenFile(m_file, FileMode.Open);
                        XmlSerializer serializer = new XmlSerializer(typeof(List<Toast>));

                        toasts = (List<Toast>)serializer.Deserialize(stream);
                    }
                    catch (Exception e)
                    {
                        System.Diagnostics.Debug.WriteLine("[Toast: Load]: " + e.Message);
                    }
                    finally
                    {
                        if (stream != null)
                        {
                            stream.Close();
                            stream.Dispose();
                        }
                    }
                }

                return toasts;
            }


            public void SaveToDisk(List<Toast> listofa)
            {
                IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
                IsolatedStorageFileStream stream = null;

                try
                {
                    stream = storage.OpenFile(m_file, FileMode.Create, FileAccess.Write);
                    XmlSerializer serializer = new XmlSerializer(typeof(List<Toast>));
                    serializer.Serialize(stream, listofa);
                }
                catch (Exception e)
                {
                    System.Diagnostics.Debug.WriteLine("[Toast: Save]: " + e.Message);
                }
                finally
                {
                    if (stream != null)
                    {
                        stream.Close();
                        stream.Dispose();
                    }
                }
            }


            public void RemoveToastNotificationsById(int id)
            {
                List<Toast> localToasts = LoadFromDisk();

                if (id == -1)
                {
                    localToasts.Clear();

                    PeriodicTask periodicTask = ScheduledActionService.Find(m_ScheduledTaskName) as PeriodicTask;
                    if (periodicTask != null)
                    {
                        ScheduledActionService.Remove(m_ScheduledTaskName);
                    }

                    SaveToDisk(localToasts);

                }
                else
                {
                    for (var i = 0; i < localToasts.Count; i++)
                    {
                        if (localToasts[i].id == id)
                        {
                            localToasts.Remove(localToasts[i]);
                            SaveToDisk(localToasts);
                        }
                    }
                }
            }
        }
    }

