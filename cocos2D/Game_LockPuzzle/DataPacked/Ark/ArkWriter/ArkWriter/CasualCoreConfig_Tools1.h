//
// CasualCoreConfig_Tools1.h
// Included by CasualCoreConfig.h, for tools specific usage of CasualCore (DLCTool)
// Liam de Koster-Kjaer
//******************************************************************************************************

#ifndef _CASUAL_CORE_CONFIG_TOOLS_H_
#define _CASUAL_CORE_CONFIG_TOOLS_H_

#define _CASUAL_CORE_CONFIG_ALTERNATE_      // to avoid default settings from CasualCoreConfig.h

//---- setting most/all flags to 0 -----

#define CC_USE_GAIA                0
//Gaia_debug.lib

#define CC_USE_SAVEGAME_INTERFACE  0
//SaveGameLib_debug.lib
//rem need CC_USE_GAIA

#define CC_USE_LIVE_MANAGER        0
//libGLXPlayerD.lib

#define CC_USE_TRACKING_MANAGER    0
//TrackingLibrary_Debug.lib

#define CC_USE_IN_APP_PURCHASE     0
//InAppPurchase_debug.lib

#define CC_USE_GLWEBTOOLS          1
//glwebtools2_debug.lib

#define CC_USE_ARK                 CC_USE_GLWEBTOOLS
//use Encryption.cpp -> glwebtools2_debug.lib

#define CC_USE_SOUND_MANAGER       0
//VoxD.lib --> +SDL.lib

#define CC_USE_GAMESWF             0
//GameSWF_d.lib + freetypeD.lib

#define CC_USE_CURL                0
//libcurl.lib [+?curl_debug.lib]

#define CC_USE_OPENSSL             0
//crypto.lib

#define CC_USE_SCRIPT_LUA          0
//Lua_d.lib

#define CC_USE_FACEBOOK_MANAGER    0
//FacebookManager.cpp

#define CC_USE_TWITTER_MANAGER     0
//TwitterManager.cpp

#endif //_CASUAL_CORE_CONFIG_TOOLS_H_

