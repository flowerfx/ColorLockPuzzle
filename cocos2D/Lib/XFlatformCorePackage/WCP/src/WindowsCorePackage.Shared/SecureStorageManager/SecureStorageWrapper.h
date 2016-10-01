#pragma once

#include <string>

#if defined(WCP_DLL)
#define WINTK_EXPORTED __declspec(dllexport)
#else
#define WINTK_EXPORTED __declspec(dllimport)
#endif

namespace WCPToolkit
{
	class SecureStorageWrapper
	{
	public:
		enum ActionType
		{
			E_ACTIONTYPE_TRYONCE = 0,				// Returns regardless of result
			//		E_ActionType_BlockUntilFinish = 1,		// Blocks normal flow until everything is ok
			//		E_ActionType_TryUntilSuccess = 2,		// Creates a thread that will try to write
			E_ACTIONTYPE_COUNT
		};

		enum Error
		{
			E_SUCCESS = 0,
			E_ERROR_UNKNOWN = 1,
			E_ERROR_CORRUPTENTRY = 2,			// PasswordVault failed to be retrieved, please try again
			E_ERROR_NOTENOUGHSPACE = 3,		// The amount of information is too big for a passwordVault entry, please remove unused info
			E_ERROR_TOOMANYENTRIES = 4,		// The password vault is probably full so there is no more space to store information
			E_ERROR_INVALIDARGUMENTS = 5,	// No argument should have length 0
			E_ERROR_NOENTRY = 6,				// The specified field is missing
		};

		WINTK_EXPORTED static Error SetValue(const std::string& key, const std::string& value, ActionType type = E_ACTIONTYPE_TRYONCE);

		// GetValue will recreate the entry so it can return a E_ERROR_TOOMANYENTRIES if there is an invalid entry in the passwordvault
		WINTK_EXPORTED static Error GetValue(const std::string& key, std::string& out_value);

		WINTK_EXPORTED static Error RemoveKey(const std::string& key, ActionType type = E_ACTIONTYPE_TRYONCE);

		WINTK_EXPORTED static Error ResetStorage();

		void DestroyInstance();

		// Functions for local usage
		WINTK_EXPORTED static Error FileSetValue(const std::string& key, const std::string& value);
		WINTK_EXPORTED static Error FileGetValue(const std::string& key, std::string& out_value);
		WINTK_EXPORTED static Error FileRemoveKey(const std::string& key);
		WINTK_EXPORTED static Error ResetFileStorage();

#ifndef OS_WP8
// Obsolete functions
// Used for compatibility reasons
		WINTK_EXPORTED static Error RawSetValue(const std::string& resource, const std::string& user, const std::string& password);
		WINTK_EXPORTED static Error RawGetValue(const std::string& resource, const std::string& user, std::string& out_password);
		WINTK_EXPORTED static Error RawIsEntry(const std::string& resource, const std::string& user);
		WINTK_EXPORTED static Error RawRemoveEntry(const std::string& resource, const std::string& user);
#endif
	};
};