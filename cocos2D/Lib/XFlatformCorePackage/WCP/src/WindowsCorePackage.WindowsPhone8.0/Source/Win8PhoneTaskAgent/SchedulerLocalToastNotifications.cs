using System;
using System.Collections.Generic;
using System.Xml.Serialization;
using System.IO; 
using System.IO.IsolatedStorage;

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
        }
    }

