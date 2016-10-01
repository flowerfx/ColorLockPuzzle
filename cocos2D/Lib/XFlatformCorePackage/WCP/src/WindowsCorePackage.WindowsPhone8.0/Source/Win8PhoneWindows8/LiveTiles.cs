using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


using System.Windows;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Scheduler;

using System.Xml.Linq;



namespace LiveTiles
{
	  
	public class LiveTile
	{
		static LiveTile m_pInstance = null;

		private List<Uri> m_tileList = new List<Uri>();


		public static LiveTile GetInstance()
		{
			if (m_pInstance == null)
			{
				m_pInstance = new LiveTile();
			}

			return m_pInstance;
		}

		public void UpdateLiveTile(bool title)
		{   
			 
			ShellTile flipTile = ShellTile.ActiveTiles.First(); 
			
			// you can force it to create like that
			// OrDefault(o => o.NavigationUri.ToString().Contains("/MainPage.xaml"));

			FlipTileData flipTileData = new FlipTileData();

			if (title)
			{
				flipTileData.Title = (string)XDocument.Load("WMAppManifest.xml").Root.Element("App").Attribute("Title");
				flipTileData.BackTitle = (string)XDocument.Load("WMAppManifest.xml").Root.Element("App").Attribute("Title"); 
			} 


			foreach (Uri t in m_tileList)
			{
				if (t.ToString().Contains("smalltile"))
					flipTileData.SmallBackgroundImage = t;

				if(t.ToString().Contains("1_widetile"))
					flipTileData.WideBackgroundImage = t;

				if(t.ToString().Contains("2_widetile"))
					flipTileData.WideBackBackgroundImage = t;

				if(t.ToString().Contains("1_mediumtile"))
					flipTileData.BackgroundImage = t;

				if (t.ToString().Contains("2_mediumtile"))
					flipTileData.BackBackgroundImage = t;
			}

			
			try
			{
				flipTile.Update(flipTileData); 
			}
			catch (System.Exception ex)
			{
				System.Diagnostics.Debug.WriteLine("Please modify your template in WMAppManifest.xml to TemplateCycle! Exception: " + ex.Message);
			} 

		}

		public void AddLiveTile(String pictureName)
		{
			if (m_tileList.Count < 9)
			{
				m_tileList.Add(new Uri(pictureName, UriKind.Relative));
			}
			else
			{
				System.Diagnostics.Debug.WriteLine("Max number of tiles is 9");
			}

		}

		public void RemoveLiveTiles()
		{

			ShellTile flipTile = ShellTile.ActiveTiles.First(); 

			FlipTileData flipTileData = new FlipTileData();

		 
			flipTileData.BackBackgroundImage = new Uri("",UriKind.Relative);
			flipTileData.WideBackBackgroundImage = new Uri("", UriKind.Relative);
			flipTileData.BackTitle = "";
			 

			flipTileData.BackgroundImage = new Uri(XDocument.Load("WMAppManifest.xml").Root.Descendants().First(x => x.Name == "BackgroundImageURI").Value, UriKind.Relative);
			flipTileData.SmallBackgroundImage = new Uri(XDocument.Load("WMAppManifest.xml").Root.Descendants().First(x => x.Name == "SmallImageURI").Value, UriKind.Relative);
			flipTileData.WideBackBackgroundImage = new Uri(XDocument.Load("WMAppManifest.xml").Root.Descendants().First(x => x.Name == "LargeBackgroundImageURI").Value, UriKind.Relative);
			
			
			flipTile.Update(flipTileData); 

		}

	}
}
