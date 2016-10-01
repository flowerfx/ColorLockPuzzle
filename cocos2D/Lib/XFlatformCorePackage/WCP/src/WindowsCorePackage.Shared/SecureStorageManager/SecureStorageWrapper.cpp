#include "pch.h"

#ifndef OS_WP8
	#include "PasswordVaultManager.h"
#endif

#include "FileStorage.h"

static std::mutex PasswordVaultMutex;

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::SetValue(const std::string& key, const std::string& value, SecureStorageWrapper::ActionType type)
{
#ifndef OS_WP8
	std::wstring local_key(key.begin(), key.end());
	std::wstring local_value(value.begin(), value.end());

	Platform::String^ managed_key = ref new Platform::String(local_key.c_str());
	Platform::String^ managed_value = ref new Platform::String(local_value.c_str());

	if (managed_key->Length() <= 0 || managed_key->Length() <= 0 || type < 0 || type >= E_ACTIONTYPE_COUNT)
	{
		return E_ERROR_INVALIDARGUMENTS;
	}

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);

	return PasswordVaultManager::GetInstance()->SetValue(managed_key, managed_value, type);
#else
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::SetValue(key, value);
#endif
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::GetValue(const std::string& key, std::string& out_value)
{
#ifndef OS_WP8
	std::wstring local_key(key.begin(), key.end());

	Platform::String^ managed_key = ref new Platform::String(local_key.c_str());
	Platform::String^ managed_value;

	if (managed_key->Length() <= 0)
	{
		return E_ERROR_INVALIDARGUMENTS;
	}

	PasswordVaultMutex.lock();
	auto ret = PasswordVaultManager::GetInstance()->GetValue(managed_key, managed_value);
	PasswordVaultMutex.unlock();

	std::wstring local_value(managed_value->Begin(), managed_value->End());
	out_value = std::string(local_value.begin(), local_value.end()); // We are only storing string values so nothing is lost in the conversion

	if (out_value.length() == 0)
	{
		return E_ERROR_NOENTRY;
	}

	return ret;
#else
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::GetValue(key,out_value);
#endif
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::RemoveKey(const std::string& key, SecureStorageWrapper::ActionType type)
{
#ifndef OS_WP8
	std::wstring local_key(key.begin(), key.end());

	Platform::String^ managed_key = ref new Platform::String(local_key.c_str());
	Platform::String^ managed_value = ref new Platform::String(L"");

	if (managed_key->Length() <= 0 || type < 0 || type >= E_ACTIONTYPE_COUNT)
	{
		return E_ERROR_INVALIDARGUMENTS;
	}

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return PasswordVaultManager::GetInstance()->SetValue(managed_key, managed_value, type);
#else

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::RemoveKey(key);
#endif
}

#ifndef OS_WP8
void WCPToolkit::SecureStorageWrapper::DestroyInstance()
{
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	WCPToolkit::PasswordVaultManager::DestroyInstance();
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::RawSetValue(const std::string& resource, const std::string& user, const std::string& password)
{
	std::wstring local_resource(resource.begin(), resource.end());
	std::wstring local_user(user.begin(), user.end());
	std::wstring local_password(password.begin(), password.end());

	Platform::String^ managed_resource = ref new Platform::String(local_resource.c_str());
	Platform::String^ managed_user = ref new Platform::String(local_user.c_str());
	Platform::String^ managed_password = ref new Platform::String(local_password.c_str());

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return PasswordVaultManager::GetInstance()->RawSetValue(managed_resource, managed_user, managed_password);
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::RawGetValue(const std::string& resource, const std::string& user, std::string& out_password)
{
	std::wstring local_resource(resource.begin(), resource.end());
	std::wstring local_user(user.begin(), user.end());

	Platform::String^ managed_resource = ref new Platform::String(local_resource.c_str());
	Platform::String^ managed_user = ref new Platform::String(local_user.c_str());
	Platform::String^ managed_password;

	PasswordVaultMutex.lock();
	auto ret = PasswordVaultManager::GetInstance()->RawGetValue(managed_resource, managed_user, managed_password);
	PasswordVaultMutex.unlock();

	std::wstring local_password(managed_password->Begin(), managed_password->End());
	out_password = std::string(local_password.begin(), local_password.end()); // We are only storing string values so nothing is lost in the conversion

	return ret;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::RawIsEntry(const std::string& resource, const std::string& user)
{
	std::wstring local_resource(resource.begin(), resource.end());
	std::wstring local_user(user.begin(), user.end());

	Platform::String^ managed_resource = ref new Platform::String(local_resource.c_str());
	Platform::String^ managed_user = ref new Platform::String(local_user.c_str());

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return PasswordVaultManager::GetInstance()->RawIsEntry(managed_resource, managed_user);
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::RawRemoveEntry(const std::string& resource, const std::string& user)
{
	std::wstring local_resource(resource.begin(), resource.end());
	std::wstring local_user(user.begin(), user.end());

	Platform::String^ managed_resource = ref new Platform::String(local_resource.c_str());
	Platform::String^ managed_user = ref new Platform::String(local_user.c_str());

	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return PasswordVaultManager::GetInstance()->RawRemoveEntry(managed_resource, managed_user);
}
#endif

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::ResetStorage()
{
#ifndef OS_WP8
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return PasswordVaultManager::GetInstance()->ResetPasswordVault();

#else
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::ResetFileStorage();
#endif
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::FileSetValue(const std::string& key, const std::string& value)
{
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::SetValue(key, value);
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::FileGetValue(const std::string& key, std::string& out_value)
{
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::GetValue(key, out_value);
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::FileRemoveKey(const std::string& key)
{
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::RemoveKey(key);
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::SecureStorageWrapper::ResetFileStorage()
{
	std::lock_guard<std::mutex> lock(PasswordVaultMutex);
	return FileStorage::ResetFileStorage();
}