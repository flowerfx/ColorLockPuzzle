include "../../premake/common/"

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------

solution "iOSCorePackage"
	
	addCommonConfig()

-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
project "iOSCorePackage"
	uuid "443F04D8-EF19-488D-B325-E3B34C0FE94F"
	
	files 
	{ 
		"../include/**.h",
		"../include/**.hpp",
		"../../iOSCorePackage_config/*.h",
		"../src/**.cpp",
		"../src/**.mm",
		"../src/**.m", 		
		"../src/**.h", 		
	}
	
	includedirs
	{ 
		"../include",
		"../../iOSCorePackage_config",
		"../src"
	}
		
	kind "StaticLib"
			
	targetname( "iOSCorePackage" )
	targetdir ("../lib/" .. GetPathFromPlatform())

	buildoptions { "-std=c++11 -stdlib=libc++ -x objective-c++ -Wno-error" }
	defines {"OS_IPHONE"}
	
	-- this is not ok, but I kept this to avoid breaking the other exernals
	local extern_path = "../../"
	configurationfiles ({ extern_path .. "iOSCorePackage_config/" .. _OPTIONS["to"] .. "/common.xcconfig" })
	
	addCommonXcodeBuildSettings()
	xcodebuildsettings
	{
		["ARCHS"] = "$(ARCHS_STANDARD_INCLUDING_64_BIT)";
		["VALID_ARCHS"] = {"armv7","armv7s", "arm64"};
		["USER_HEADER_SEARCH_PATHS"] = "$(COMMON_HEADER_SEARCH_PATHS)";
	}
	
	-- // USE .XCCONFIG FILES
	-- activate default .xcconfig files feature for all configurations
	generatexcconfigs "YES"
	
	local extern_path_1 = "../../../"
	-- setting these 'user' .xcconfig files we overwrite the default ones
	
	configurationfiles
	{ 
		extern_path_1 .. "iOSCorePackage_config/" .. _OPTIONS["to"] .. "/xcconfigs_iOSCorePackage/user-debug-project.xcconfig",
		extern_path_1 .. "iOSCorePackage_config/" .. _OPTIONS["to"] .. "/xcconfigs_iOSCorePackage/user-release-project.xcconfig",
		extern_path_1 .. "iOSCorePackage_config/" .. _OPTIONS["to"] .. "/xcconfigs_iOSCorePackage/user-debug-target.xcconfig",
		extern_path_1 .. "iOSCorePackage_config/" .. _OPTIONS["to"] .. "/xcconfigs_iOSCorePackage/user-release-target.xcconfig",
	}				  
	-- //
	
	configuration "../src/FileSystem/FileSystem.cpp"
	language "ObjCpp"
	
	
	
	
	
-------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------
project "test"

	kind "WindowedApp"

	files 
	{
		 "../tests/**",
		 "Info.plist",
	}

	includedirs
	{
		"../include/",		
	}
	
	buildoptions { "-std=c++11 -stdlib=libc++ -x objective-c++ -Wno-error" }	
	defines {"OS_IPHONE"}
	
	addCommonXcodeBuildSettings()	
	xcodebuildsettings 
	{
		["CODE_SIGN_IDENTITY"] = "iPhone Developer: Valeri Vuchov (WDTMDP2J2J)",
	}		

	targetdir ("../build/" .. GetPathFromPlatform())	
	
	linkoptions 
	{
		"-framework CoreFoundation",
		"-framework Foundation",
		"-framework OpenGLES",
		"-framework UIKit",
		"-framework QuartzCore",
		"-framework CoreMotion",
		"-framework CoreTelephony",
		"-framework CoreLocation", 
		"-framework Security",
		"-framework AdSupport",
		"-framework GameKit",
		"-framework CFNetwork",
		"-framework MobileCoreServices",
		"-framework Accounts",
		"-framework Social",
		"-framework SystemConfiguration",
		"-framework MessageUI",
		"-framework CoreGraphics",
		"-framework CoreAudio",
		"-framework AudioToolbox",
		"-framework AddressBook",
		"-framework MediaPlayer",
	}

	links { "iOSCorePackage" }
	
	configuration "Debug"
		targetsuffix( "_d" )

	configuration "Release" 
