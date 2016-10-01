#pragma once
// Please do not use this file directly
// Use the functions from SecureStorageWrapper

#include "SecureStorageWrapper.h"
#include "ISingleton.h"

namespace WCPToolkit
{
	class PasswordVaultManager : public WCPToolkit::ISingleton < PasswordVaultManager >
	{
	private:

		SecureStorageWrapper::Error SetValue(Platform::String^ key, Platform::String^ value, SecureStorageWrapper::ActionType type = SecureStorageWrapper::E_ACTIONTYPE_TRYONCE);
		SecureStorageWrapper::Error GetValue(Platform::String^ key, Platform::String^& out_value);
		SecureStorageWrapper::Error ResetPasswordVault();

// Obsolete functions
		SecureStorageWrapper::Error RawSetValue(Platform::String^ resource, Platform::String^ user, Platform::String^ password);
		SecureStorageWrapper::Error RawGetValue(Platform::String^ resource, Platform::String^ user, Platform::String^& out_password);
		SecureStorageWrapper::Error RawIsEntry(Platform::String^ resource, Platform::String^ user);
		SecureStorageWrapper::Error RawRemoveEntry(Platform::String^ resource, Platform::String^ user);

		PasswordVaultManager();
		virtual ~PasswordVaultManager();

		// GetCurrentPassword will retrieve the entry on which the operation will be made
		// it will also regenerate the entry in the password vault and delete any bad entry ( partial identifiers )
		SecureStorageWrapper::Error PasswordVaultManager::GetCurrentPassword(Platform::String^& current_password);

		Platform::String^ m_machineId;
		Platform::String^ m_localIdentifier;

		friend class WCPToolkit::ISingleton < PasswordVaultManager >;
		friend class WCPToolkit::SecureStorageWrapper;
	};
};