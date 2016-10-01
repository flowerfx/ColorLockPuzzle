#include "pch.h"

#include "SecureStorageWrapper.h"

namespace WCPToolkit
{
	namespace FileStorage
	{
		SecureStorageWrapper::Error SetValue(const std::string& key, const std::string& value, SecureStorageWrapper::ActionType type = SecureStorageWrapper::E_ACTIONTYPE_TRYONCE);

		SecureStorageWrapper::Error GetValue(const std::string& key, std::string& out_value);

		SecureStorageWrapper::Error RemoveKey(const std::string& key, SecureStorageWrapper::ActionType type = SecureStorageWrapper::E_ACTIONTYPE_TRYONCE);

		SecureStorageWrapper::Error ResetFileStorage();
	}
}