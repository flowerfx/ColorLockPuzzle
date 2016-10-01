/*******************************************************/
/*******************************************************/
/*******************************************************/
/* WARNING:  DO NOT INCLUDE THIS FILE                  */
/* WARNING:  Use #include "Platform/Platform.h" instead*/
/*******************************************************/
/*******************************************************/
/*******************************************************/
#include "PlatformBaseInterface/PlatformBase.h"

#include "Platform/Platform.h"
#include "Platform/GeoLocator.h"
#include "Platform/DeviceInfo.h"
#include "Platform/FileSystem.h"
#include "Platform/WIFIInfo.h"


namespace platform
{	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	Settings::Settings()
	{
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////





	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	GeoLocatorBase::Location::Location() :latitude(0.0), longitude(0.0), altitude(0.0), accuracy(0.0)
	{
	}

	void GeoLocatorBase::Enable()
	{
	}
	
	void GeoLocatorBase::Disable()
	{
	}
		
	GeoLocatorBase::State GeoLocatorBase::GetState() 
	{ 
		return FEATURE_DISABLED; 
	}
		
	bool GeoLocatorBase::IsEnabled()
	{ 
		return false; 
	}
		
	void GeoLocatorBase::SetConfirmationText(const std::string& text)
	{
	}
		
	bool GeoLocatorBase::HasValidCoordinates()
	{ 
		return false; 
	}
		
	GeoLocatorBase::Location GeoLocatorBase::GetLocation()
	{ 
		return Location(); 
	}

	GeoLocator* GeoLocatorBase::CreateSingleInstance(Settings* settings)
    {
		std::lock_guard<std::mutex> lock(s_mutex);
		if (s_instance)
			return nullptr; /// return nullptr because there is already an instance!

		s_instance = new GeoLocator();
		((GeoLocatorBase*)s_instance)->m_settings = settings;
		return s_instance;
    }

	void GeoLocatorBase::DeleteInstance()
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		delete s_instance;
		s_instance = nullptr;
	}

	GeoLocatorBase::GeoLocatorBase() :m_settings(nullptr) 
	{
	}

	GeoLocatorBase::~GeoLocatorBase()
	{ 
			
	};

	std::mutex GeoLocatorBase::s_mutex;

	GeoLocator* GeoLocatorBase::s_instance = nullptr;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////


	



	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	DeviceInfoBase::ScreenRes::ScreenRes() 
								: w(0)
								, h(0)
	{
	}

	std::string DeviceInfoBase::GetModel()
	{ 
		return "none"; 
	}

	std::string DeviceInfoBase::GetCarrier() 
	{ 
		return "none"; 
	}

	std::string DeviceInfoBase::GetCountryCode() 
	{ 
		return "XX"; 
	}

	std::string DeviceInfoBase::GetLanguage() 
	{ 
		return "none"; 
	}

	//virtual std::string GetGLDID() { return "none"; }

	std::string DeviceInfoBase::GetOSVersion() 
	{ 
		return "none"; 
	}

	::uint32_t DeviceInfoBase::GetScreenW() 
	{ 
		return 0; 
	}

	::uint32_t DeviceInfoBase::GetScreenH() 
	{ 
		return 0; 
	}

	DeviceInfoBase::ScreenRes DeviceInfoBase::GetScreenRes() 
	{ 
		return ScreenRes(); 
	}

	DeviceInfoBase::ConnectivityType DeviceInfoBase::GetConnectivityType() 
	{ 
		return ConnectivityType::_NOT_IMPLEMENTED; 
	}


	DeviceInfo* DeviceInfoBase::CreateSingleInstance(Settings* settings)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (s_instance)
			return nullptr; /// return nullptr because there is already an instance!

		s_instance = new DeviceInfo();
		((DeviceInfoBase*)s_instance)->m_settings = settings;
		return s_instance;
    }

	void DeviceInfoBase::DeleteInstance()
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		delete s_instance;
		s_instance = nullptr;
	}

	DeviceInfoBase::DeviceInfoBase() :m_settings(nullptr)
	{
	}
	DeviceInfoBase::~DeviceInfoBase()
	{ 		
	}

	std::mutex DeviceInfoBase::s_mutex;

	DeviceInfo* DeviceInfoBase::s_instance = nullptr;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////






	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	int FileSystemBase::Rename(const std::string& oldFileName, const std::string& newFileName)
	{ 
		return -1; 
	}

	int FileSystemBase::Remove(const std::string& fileName) 
	{ 
		return -1; 
	}

	FILE* FileSystemBase::OpenFile(const std::string& fileName, const std::string& mode) 
	{ 
		return nullptr; 
	}

	int FileSystemBase::CloseFile(FILE* file) 
	{ 
		return 0; 
	}

	int FileSystemBase::GetFileSize(const std::string& fileName, off_t& size) 
	{ 
		return -1; 
	}

	std::string FileSystemBase::GetStorageFilePath() 
	{ 
		return std::string(); 
	}

	std::string FileSystemBase::GetSaveFilePath() 
	{ 
		return std::string(); 
	}

	FileSystem* FileSystemBase::CreateSingleInstance(Settings* settings)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (s_instance)
			return nullptr; /// return nullptr because there is already an instance!

		s_instance = new FileSystem();
		((FileSystemBase*)s_instance)->m_settings = settings;
		return s_instance;
	}

	void FileSystemBase::DeleteInstance()
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		delete s_instance;
		s_instance = nullptr;
	}

	FileSystemBase::FileSystemBase():m_settings(nullptr)
	{
	}
	FileSystemBase::~FileSystemBase()
	{			
	};

	std::mutex FileSystemBase::s_mutex;

	FileSystem* FileSystemBase::s_instance = nullptr;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

	




	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	std::string WIFIInfoBase::GetInfoAsJson()
	{
		return std::string("{}");
	}

	void WIFIInfoBase::Enable()
	{
	}

	void WIFIInfoBase::Disable()
	{
	}

	WIFIInfoBase::State WIFIInfoBase::GetState()
	{
		return FEATURE_DISABLED;
	}

	bool WIFIInfoBase::IsEnabled()
	{
		return false;
	}

	WIFIInfo* WIFIInfoBase::CreateSingleInstance(Settings* settings)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (s_instance)
			return nullptr; /// return nullptr because there is already an instance!

		s_instance = new WIFIInfo();
		((WIFIInfoBase*)s_instance)->m_settings = settings;
		return s_instance;
	}

	void WIFIInfoBase::DeleteInstance()
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		delete s_instance;
		s_instance = nullptr;
	}

	WIFIInfoBase::WIFIInfoBase() :m_settings(nullptr)
	{
	}
	WIFIInfoBase::~WIFIInfoBase()
	{
	};

	std::mutex WIFIInfoBase::s_mutex;

	WIFIInfo* WIFIInfoBase::s_instance = nullptr;
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////








	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////
	GeoLocator* PlatformBaseInterface::GetGeoLocator()
	{ 
		return m_geolocator; 
	}

	DeviceInfo* PlatformBaseInterface::GetDeviceInfo()
	{ 
		return m_deviceInfo; 
	}

	FileSystem* PlatformBaseInterface::GetFileSystem()
	{ 
		return m_filesystem; 
	}

	WIFIInfo* PlatformBaseInterface::GetWIFIInfo()
	{
		return m_wifiInfo;
	}


	std::string PlatformBaseInterface::GenerateUUID()
	{ 
		return ""; 
	}


	std::shared_ptr<PlatformImpl> PlatformBaseInterface::CreateSingleInstance(const Settings& settings)
	{
		std::lock_guard<std::mutex> lock(s_mutex);
		if (!s_instance.expired())
			return nullptr; /// return nullptr because there is already an instance!
			
		std::shared_ptr<PlatformImpl> instance = std::shared_ptr<PlatformImpl>(new PlatformImpl(), PlatformBaseInterface::deleter());
		instance->m_settings = settings;
		s_instance = instance;
		return instance;
	}

	PlatformBaseInterface::PlatformBaseInterface()
	{
		m_geolocator = GeoLocator::CreateSingleInstance(&m_settings);
		m_deviceInfo = DeviceInfo::CreateSingleInstance(&m_settings);
		m_filesystem = FileSystem::CreateSingleInstance(&m_settings);
		m_wifiInfo = WIFIInfo::CreateSingleInstance(&m_settings);
	}
		
	PlatformBaseInterface::~PlatformBaseInterface()
	{
		std::lock_guard<std::mutex> lock(s_mutex); 

		GeoLocatorBase::DeleteInstance();
		DeviceInfoBase::DeleteInstance();
		FileSystemBase::DeleteInstance();
		WIFIInfoBase::DeleteInstance();
	}

	void PlatformBaseInterface::deleter::operator()(PlatformImpl* p)
	{ 
		delete p; 
	}
				

	std::mutex PlatformBaseInterface::s_mutex;

	std::weak_ptr<PlatformImpl> PlatformBaseInterface::s_instance = std::weak_ptr<PlatformImpl>();
	/////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////

} //platform




 
 
 
 
 
 