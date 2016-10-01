using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace AntiHack
{
    class AntiHack
    {
        private StringBuilder m_TestString;
        private bool m_IsStringConstructed;
        private StringBuilder m_DllSizes;
        private string m_Publisher, m_PublisherId;

        private static AntiHack s_Instance = null;
        private static bool s_IsInitialized = false;

        private AntiHack()
        {
            m_IsStringConstructed = false;

            ParseManifest();
        }

        private long GetBinarySize(string file)
        {
            var stream = File.OpenRead(file);

            string[] temp = file.Split('\\');

            BinaryReader br = new BinaryReader(stream);
            long size = stream.Length;

            stream.Close();

            return size;
        }

        private void ParseManifest()
        {
            System.Xml.Linq.XElement manifest = System.Xml.Linq.XElement.Load("WMAppManifest.xml");
            var element = (from manifestData in manifest.Descendants("App") select manifestData).SingleOrDefault();

            m_Publisher = element.Attribute("Publisher").Value;
            m_PublisherId = element.Attribute("PublisherID").Value;
        }

        private void ComposeSizes(string dllPath, out StringBuilder sizes)
        {
            string[] fileEntries = Directory.GetFiles(dllPath);

            m_TestString = new StringBuilder("");

            sizes = new StringBuilder("");

            foreach (string file in fileEntries)
            {
                if (file.Contains(".dll"))
                {
                    long size = GetBinarySize(file);

                    sizes.Append(size.ToString() + " ");
                }
            }

            sizes.Remove(sizes.Length - 1, 1);

            byte[] hashes = new byte[m_TestString.Length * sizeof(char)];
            System.Buffer.BlockCopy(m_TestString.ToString().ToCharArray(), 0, hashes, 0, hashes.Length);

            
        }

        public string GetTestString()
        {
            string dllPath = Windows.ApplicationModel.Package.Current.InstalledLocation.Path;
            if (!m_IsStringConstructed)
            {
                m_TestString = new StringBuilder("");
                ComposeSizes(dllPath, out m_DllSizes);

                m_IsStringConstructed = true;

                m_TestString.Append(m_Publisher + "|" + m_PublisherId + "|");
                m_TestString.Append("(" + m_DllSizes + ")|");
                m_TestString.Append(DateTime.Now.Ticks);
            }

            return m_TestString.ToString();
        }

        public static AntiHack GetInstance()
        {
            if (!s_IsInitialized)
            {
                s_Instance = new AntiHack();
                s_IsInitialized = true;
            }

            return s_Instance;
        }
    }
}
