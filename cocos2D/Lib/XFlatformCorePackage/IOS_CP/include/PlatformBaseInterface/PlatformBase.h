#ifndef __PLATFORMBASE_H__
#define __PLATFORMBASE_H__

#include <string>
#include <memory>
#include <mutex>

#if !defined(PLATFORM_IMPL)
#	error Do not include PlatformBase.h(this file) or PlatformBase.hpp directly. Use #include "Platform/Platform.h"
#endif


/** WINTK_EXPORTED is used only for Windows8 platforms to mark which functions must be exported.
* In a library that must implement this interface WCP_DLL must be declared. 
* In a library/app that must, ONLY, use this interface, do not declare WCP_DLL!*/
#if defined(WINAPI_FAMILY) && ((WINAPI_FAMILY==WINAPI_FAMILY_APP) || (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP))
#	if defined(WCP_DLL)
#		define WINTK_EXPORTED __declspec(dllexport)
#	else
#		define WINTK_EXPORTED __declspec(dllimport)
#	endif
#else
#	define WINTK_EXPORTED
#endif


/** PLATFORMBASE_DEPRECATED is a define used to mark if some functions are deprecated. */
#ifdef __GNUC__
#	define PLATFORMBASE_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#	if defined(WINAPI_FAMILY) && ((WINAPI_FAMILY==WINAPI_FAMILY_APP) || (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP))
#		define PLATFORMBASE_DEPRECATED(func) func
#	else
#		define PLATFORMBASE_DEPRECATED(func) __declspec(deprecated) func
#	endif
#else
#	pragma message("WARNING: You need to implement PLATFORMBASE_DEPRECATED for this compiler!")
#	define PLATFORMBASE_DEPRECATED(func) func
#endif


namespace platform
{	
	class PlatformBaseInterface;
	class GeoLocator;
	class DeviceInfo;
	class FileSystem;
	class WIFIInfo;
	class PlatformImpl;
	

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/** TODO: add here members that means something for platforms (for example product id, or GGI)*/
	struct WINTK_EXPORTED Settings
	{
		Settings();
	};


	/////////////////////////////////////////////////////////////////////////////////////////////////
	/** This is the Interface that must be implemented by the Frameworks (aka AndroidFramework, 
	* Windows8Framework, iOSCorePackage, Win32CorePackage and tvOSCorePackage), to have the same interface for
	* the GeoLocator service.*/
	class GeoLocatorBase
	{
		friend class PlatformBaseInterface;
	public:
	
		enum State: ::int32_t
		{
			/** This means that the feature was disabled at compile time by using USE_GEOLOCATION=0 flag.*/
			FEATURE_DISABLED = 0,
			
			/** This means that the app did not called Enable() or just called Disable()*/
			DISABLED,
			
			/** This means that the function Enabled() was called but did not have valid coordinates yet. */
			ENABLED, 
			
			/** This means that the GeoLocator is enabled and has valid coordinates */
			HAS_VALID_COORDINATES,

			/** This means that the GeoLocator is enabled but the coordinates were not updated (not received in a given time).
			* When Air-plane mode is on the coordinates will expire after a given time.*/
			HAS_EXPIRED_COORDINATES,

			/** This means that the GeoLocator is enabled (aka Enable() function was called) but the coordinates 
			* cannot be fetches because the OS denied the access to geo location;
			* In most cases this is because the user denied the access for geo location for the current app;*/
			HAS_ACCESS_DENIED,
			
			/** This means that the user disablet the location service from the device OS settings. */
			HAS_LOCATION_SERVICE_DISABLED,
			
			/** This means that the GeoLocator is enabled but unknown errors was received */
			HAS_ERRORS
		};
	
		/** The structure that store the location obtained from GeoLocator. */
		struct Location
		{
			double latitude;
			double longitude;
			double altitude;
			double accuracy;	
			
		    /** The constructor must exists to initialize the members with the default values.*/
			WINTK_EXPORTED Location();
		};

		/// ----------- Functions that can/must be implemented -----------------------------

		/** This will enable the service. The OS can/will prompt the user with a question 
		regarding this action. The 'State' of the service will be ENABLED. */
		virtual void Enable();
		
		/** This will Disable the service. The 'State' of the service will be DISABLED. */
		virtual void Disable();
		
		/** This is used to get the state of GeoLocator. 
		* @return the value meaning the 'State' of geo locator system.*/
		virtual State GetState();
		
		/** Use this to check if the service was enabled or not. After you call Enabled(), the OS will 
		* initialize the service. Service is enabled if the 'State' is not DISABLED => Any other value 
		* (HAS_EXPIRED_COORDINATES, HAS_ACCESS_DENIED, etc...) means that the geo locator is enabled.
		* @return true if the user enabled the geo locator (aka Enabled() was called).*/
		virtual bool IsEnabled();
		
		/** Use this to set the text that can/will be displayed in the OS popup message box 
		* @param text is a const ref to the string contains the message */
		virtual void SetConfirmationText(const std::string& text);
		
		/** Use this to check if the GeoLocator can retrieve valid coordinates. The locations cannot be available from the moment 
		* when the service is enabled, so , we need to check if the coordinates are valid.
		* @return true if the coordinates are valid. */
		virtual bool HasValidCoordinates();
		
		/** Use it to get the Location. This function will return the coordonates: latitude, longitude, altitude(is 0) and accuracy.
		* @return a structure of Location type. */
		virtual Location GetLocation();

		/// --------------------------------------------------------------------------------


	protected:

        /** Use this to get the single instance for the GeoLocator. This function will fail if the 
		* previous instance is not deleted => You cannot have two different instances at a same time.
		* If you delete this pointer, the app will crash, SO do not store or delete it. The memory will
		* be freed by PlatformBaseInterface.
		* @return a raw for GeoLocator */
		WINTK_EXPORTED static GeoLocator* CreateSingleInstance(Settings* settings);

		/** Delete the GeoLocator Instance */
		WINTK_EXPORTED static void DeleteInstance();

	protected:
		/** This is used to access the settings from the platform */
		Settings* m_settings;
		
	protected:
		/** The constructor is protected, so you cannot create the instance of this class.
		* You can obtain the pointer from platform->GetGeoLocator() */
		GeoLocatorBase() ;

		/** The destructor */
		virtual ~GeoLocatorBase();

	private:

		/** mutex that protects the access to the s_instance*/
		static std::mutex s_mutex;

		/** single instance that can exists at a time. Is not a singleton! */
		static GeoLocator* s_instance;
	};

	





	/////////////////////////////////////////////////////////////////////////////////////////////////
	/** This is the Interface that must be implemented by the Frameworks (aka AndroidFramework,
	* Windows8Framework, iOSCorePackage, Win32CorePackage and tvOSCorePackage), to have the same interface for
	* the DeviceInfo .*/
	class DeviceInfoBase
	{
		friend class PlatformBaseInterface;
	public:
	
		/// ----------- Functions that can/must be implemented -----------------------------

		/** Use this to retrieve the Device/Platform model. 
		* @return a string with the device model. If the string is 
		*         'none' means that the platform does not implement this function.*/
		virtual std::string GetModel();

		/** Use this to retrieve the Carrier.
		* @return a string with the Carrier. If the string is
		*         'none' means that the platform does not implement this function.*/
		virtual std::string GetCarrier();

		/** Use this to retrieve the country code from device.
		* Is used standard ISO-3166-1-alpha-2 (aka two letters uppercase).
		* The implementation of this function must return "XX" if the country
		* code cannot be detected or is in other format.
		* @return a two letters string equivalent with the country code. */
		virtual std::string GetCountryCode();

		/** Use this to retrieve the Language.
		* @return a string with the Language. If the string is
		*         'none' means that the platform does not implement this function.*/
		virtual std::string GetLanguage();

		/** Use this to retrieve the Gameloft Device Identifier (GLDID).
		* @return a string with the GLDDID. If the string is
		*         'none' means that the platform does not implement this function.*/
		//virtual std::string GetGLDID() { return "none"; }

		/** Use this to retrieve the OS Version.
		* @return a string with the OS Version. If the string is
		*         'none' means that the platform does not implement this function.*/
		virtual std::string GetOSVersion() ;

		/** Use this to retrieve the screen Width. Thsi function is deprecated! Use GetScreenRes() instead!
		* @return an unsigned int that means the width of the screen*/
		virtual ::uint32_t PLATFORMBASE_DEPRECATED(GetScreenW());

		/** Use this to retrieve the screen Height. Thsi function is deprecated! Use GetScreenRes() instead!
		* @return an unsigned int that means the height of the screen*/
		virtual ::uint32_t PLATFORMBASE_DEPRECATED(GetScreenH());

		/** The ScreenRes is used to encapsulate the width and height of the screen. 
		The W & H are in pixels measured starting from left top corner.*/
		struct ScreenRes 
		{ 
			::uint32_t w; 
			::uint32_t h; 
			ScreenRes();
		};

		/** Use this to get the resolution of default screen. By default (if a platform did 
		not implement this function) will be returned w=0 and h=0. */
		virtual ScreenRes GetScreenRes();


		/** This enum is used to store all possible connectivity types */
		enum ConnectivityType : ::int32_t
		{
			_NOT_IMPLEMENTED = 0,
			_NO_CONNECTIVITY,
			_UNKNOWN_CONNECTIVITY,
			_2G_CONNECTIVITY,
			_3G_CONNECTIVITY,
			_4G_CONNECTIVITY,
			_BLUETOOTH_CONNECTIVITY,
			_ETHERNET_CONNECTIVITY,
			_MOBILE_CONNECTIVITY,
			_WIFI_CONNECTIVITY,
			_WIMAX_CONNECTIVITY,
		};

		/** Use this to get the connectivity type that a device have at a moment. Only the default 
		(active) connectivity type will be returned. If a platform does not implement this 
		function, the _NOT_IMPLEMENTED will be returned.*/
		virtual ConnectivityType GetConnectivityType();
		/// --------------------------------------------------------------------------------


	protected:

        /** Use this to get the single instance for the DeviceInfo. This function will fail if the 
		* previous instance is not deleted => You cannot have two different instances at a same time.
		* @return a raw for DeviceInfo */
		WINTK_EXPORTED static DeviceInfo* CreateSingleInstance(Settings* settings);

		/** Use this to delete the instance created with CreateSingleInstance */
		WINTK_EXPORTED static void DeleteInstance();

	protected:

		/** A pointer to access the settings from PlatformBaseInterface*/
		Settings* m_settings;

	protected:

		/** The constructor cannot be accessed. Use CreateSingleInstance to get a pointer to this class.*/
		DeviceInfoBase();

		/** The destructor.*/
		virtual ~DeviceInfoBase();

	private:

		/** Protect the destructro & constructor using this mutex.*/
		static std::mutex s_mutex;

		/** The single instance is stored in s_instance.*/
		static DeviceInfo* s_instance;
	};

	





	/////////////////////////////////////////////////////////////////////////////////////////////////
	/** This is the Interface that must be implemented by the Frameworks (aka AndroidFramework,
	* Windows8Framework, iOSCorePackage, etc...), to have the same interface for
	* the FileSystem features.*/
	class FileSystemBase
	{
		friend class PlatformBaseInterface;

	public:

		/// ----------- Functions that can/must be implemented -----------------------------

		/** Is a wrapper over the remove function - http://www.cplusplus.com/reference/cstdio/rename/?kw=rename
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param oldFileName string containing the name of an existing file to be renamed and/or moved.Its value shall
		*        follow the file name specifications of the running environment and can include a path (if supported by the system).
		* @param newFileName string containing the new name for the file. Its value shall follow the file name
		*        specifications of the running environment and can include a path (if supported by the system).
		* @return 0 on success, on error -1 is returned and errno is set properly*/
		virtual int Rename(const std::string& oldFileName, const std::string& newFileName);

		/** Remove is a wrapper over the remove function - http://www.cplusplus.com/reference/algorithm/remove/?kw=remove
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param fileName is the name of the file which is desired to remove
		* @return 0 on success, on error -1 is returned and errno is set properly*/
		virtual int Remove(const std::string& fileName);

		/** OpenFile is a wrapper over the fopen function - http://www.cplusplus.com/reference/cstdio/fopen/?kw=fopen
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param fileName is a string containing the name of the file to be opened.
		* @param mode string containing a file access mode
		* @return a pointer to a FILE on operation successfully, otherwise return a null pointer*/
		virtual FILE* OpenFile(const std::string& fileName, const std::string& mode);

		/** OpenFile is a wrapper over the fclose function - http://www.cplusplus.com/reference/cstdio/fclose/?kw=fclose
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param file is a pointer to a FILE object that specifies the stream to be closed.
		* @return zero value if the stream is successfully closed, otherwise a EOF is returned.*/
		virtual int CloseFile(FILE* file);

		/** Use this function to return the size of the file
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		*@return On success, zero is returned. On error, -1 is returned.*/
		virtual int GetFileSize(const std::string& fileName, off_t& size);

		/** Use this to get the platform dependent path location where files (related with this app) are stored.
		* @return a string containing the absolute path with '/' or '\\' at the end.*/
		virtual std::string GetStorageFilePath();

		/** Use this to get the platform dependent path location where you can save some temporary files.
		* @return a string containing the absolute path with / at the end .*/
		virtual std::string GetSaveFilePath();
		/// --------------------------------------------------------------------------------


	protected:

		/** Use this to get the single instance for the DeviceInfo. This function will fail if the
		* previous instance is not deleted => You cannot have two different instances at a same time.
		* @return a raw for DeviceInfo */
		WINTK_EXPORTED static FileSystem* CreateSingleInstance(Settings* settings);

		/** Use this to delete the instance. */
		WINTK_EXPORTED static void DeleteInstance();

	protected:

		/** A pointer to access the settings from PlatformBaseInterface*/
		Settings* m_settings;

	protected:

		/** The constructor cannot be accessed. Use CreateSingleInstance to get a pointer to this class.*/
		FileSystemBase();

		/** The destructor. */
		virtual ~FileSystemBase();
	private:

		/** Protect the destructro & constructor using this mutex.*/
		static std::mutex s_mutex;

		/** The single instance is stored in s_instance.*/
		static FileSystem* s_instance;
	};








	/////////////////////////////////////////////////////////////////////////////////////////////////
	/** This is the Interface that must be implemented by the Frameworks (aka AndroidFramework,
	* Windows8Framework, iOSCorePackage, etc...), to have the same interface for
	* the WIFI service.*/
	class WIFIInfoBase
	{
		friend class PlatformBaseInterface;
	public:

		enum State : ::int32_t
		{
			/** This means that the feature was disabled at compile time by using USE_WIFI=0 flag.*/
			FEATURE_DISABLED = 0,

			/** This means that the app did not called Enable() or just called Disable()*/
			DISABLED,

			/** This means that the function Enabled() was called. */
			ENABLED,

			/** This means that the WIFI service is enabled but unknown errors was encountered */
			HAS_ERRORS
		};

		/// ----------- Functions that can/must be implemented -----------------------------

		/** This will enable the service. The OS can/will prompt the user with a question
		regarding this action. The status of the service will be ENABLED. */
		virtual void Enable();

		/** This will Disable the service. The status of the service will be DISABLED. */
		virtual void Disable();

		/** This is used to get the state of WIFI Info service.
		* @return the value meaning the state of WIFI Info.*/
		virtual State GetState();

		/** Use this to check if the service was enabled or not. After you call Enabled(), the OS will
		* initialize the service.
		* @return true if the service was enabled.*/
		virtual bool IsEnabled();

		/** Use it to get the WIFI info. The data will be returned as a serialized json in a string.
		* By default, if this function is not implemented, the string returned will be "{}" !
		* @return a standard string containing a serialized json. */
		virtual std::string GetInfoAsJson();

		/// --------------------------------------------------------------------------------

	protected:

		/** Use this to get the single instance for the DeviceInfo. This function will fail if the
		* previous instance is not deleted => You cannot have two different instances at a same time.
		* @return a raw for WIFIBase */
		WINTK_EXPORTED static WIFIInfo* CreateSingleInstance(Settings* settings);

		/** Use this to delete the instance. */
		WINTK_EXPORTED static void DeleteInstance();

	protected:

		/** A pointer to access the settings from PlatformBaseInterface*/
		Settings* m_settings;

	protected:

		/** The constructor cannot be accessed. Use CreateSingleInstance to get a pointer to this class.*/
		WIFIInfoBase();

		/** The destructor. */
		virtual ~WIFIInfoBase();
	private:

		/** Protect the destructro & constructor using this mutex.*/
		static std::mutex s_mutex;

		/** The single instance is stored in s_instance.*/
		static WIFIInfo* s_instance;
	};








	/////////////////////////////////////////////////////////////////////////////////////////////////
    /** This is the main Interface used to abstractize the platforms. This is a template, that must be derived 
    * and implemented by each *Framework/*CorePackage. */
	class WINTK_EXPORTED PlatformBaseInterface
	{
	public:

		/// ----------- Functions that can/must be implemented -----------------------------

		/** This will generate an Universally unique identifier aka UUID. See https://en.wikipedia.org/wiki/Universally_unique_identifier
		* @return a string containing the UUID or an empty string if the function was not implemented.*/
		virtual std::string GenerateUUID();

		/// --------------------------------------------------------------------------------


		/** Getter for GeoLocator service. Do not store the result for a later usage (can be invalid later if and 
        * only if PlatformBaseInterface is released). Always use this function to access the GeoLocator service!
		* @return a raw pointer to access the GeoLocator */
		GeoLocator* GetGeoLocator();

		
		/** Getter for Device Info. Do not store the result for a later usage (can be invalid later if and 
        * only if PlatformBaseInterface is released). Always use this function to access the DeviceInfo!
		* @return a raw pointer to access the DeviceInfo */
		DeviceInfo* GetDeviceInfo();


		/** Getter for File System. Do not store the result for a later usage (can be invalid later if and 
        * only if PlatformBaseInterface is released). Always use this function to access the FileSystem! 
		* @return a raw pointer to access the FileSystem */
		FileSystem* GetFileSystem();


		/** Getter for File System. Do not store the result for a later usage (can be invalid later if and
		* only if PlatformBaseInterface is released). Always use this function to access the WIFIInfo!
		* @return a raw pointer to access the WIFIInfo */
		WIFIInfo* GetWIFIInfo();

	private:
		/** This deleter class is a helper to allow the shared_ptr to access private destructors */
		class deleter;
		friend class deleter;
		class deleter
		{
		public:
			void operator()(PlatformImpl* p);
		};

	public:		
		/** Use this to get the single instance for the GeoLocator. This function will fail if the
		* previous instance is not deleted => You cannot have two different instances at a same time.
		* @return a shared_ptr for Platform */
		static std::shared_ptr<PlatformImpl> CreateSingleInstance(const Settings& settings);

	protected:
		PlatformBaseInterface();
		
		virtual ~PlatformBaseInterface();
		
	protected:
		Settings m_settings;
		GeoLocator* m_geolocator;
		DeviceInfo* m_deviceInfo;
		FileSystem* m_filesystem;
		WIFIInfo* m_wifiInfo;

	private:
		static std::mutex s_mutex;
		static std::weak_ptr<PlatformImpl> s_instance;
	};
} //platform

#endif //__PLATFORMBASE_H__




 
 
 
 
 
 