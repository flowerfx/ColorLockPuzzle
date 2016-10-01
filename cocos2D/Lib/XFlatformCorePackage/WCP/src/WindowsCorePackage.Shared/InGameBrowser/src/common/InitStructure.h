#pragma once

namespace WCPToolkit
{
    namespace InGameBrowserLibrary
    {
        public ref class InitParams sealed
        {
		public:
			property Platform::String^ Language;
			property Platform::String^ AnonymousAccount;
			property Platform::String^ FacebookId;
			property Platform::String^ GLiveAccount;
			property Platform::String^ GameCode;
			property Platform::String^ GameOperator;
			property Platform::String^ GameVersion;
			property Platform::String^ HDIDFV;
			property Platform::String^ BaseUrl;
			property Platform::String^ ClientID;
			property Platform::String^ WAID;
			property Platform::String^ XboxID;
			property int UserAge;
			property Platform::String^ OsVersion;

			InitParams();
          

		internal:
			bool CheckParams();        
		};
    }
}
