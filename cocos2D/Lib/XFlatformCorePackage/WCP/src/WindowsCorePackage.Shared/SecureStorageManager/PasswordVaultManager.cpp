#include "pch.h"

#include "PasswordVaultManagerConfig.h"
#include "PasswordVaultManager.h"
#include "PasswordVaultEntry.h"

#include <fstream>
#include <guiddef.h>
#include <objbase.h>

WCPToolkit::PasswordVaultManager::PasswordVaultManager()
{
	std::wstring LocalPath(Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data());
	std::string MagicFile(LocalPath.begin(), LocalPath.end());

	MagicFile = MagicFile + "\\" + MAGIC_FILE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	wchar_t local_data[MAX_GUID];

#if defined(OS_WP81)
	auto token = Windows::System::Profile::HardwareIdentification::GetPackageSpecificToken(nullptr);
	if (token != nullptr && token->Id != nullptr)
	{
		m_machineId = Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(token->Id);
	}
	else
	{
		m_machineId = "WidowsPhone81";
	}
#else
	auto clientDeviceInformation = ref new Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation();
	m_machineId = clientDeviceInformation->Id.ToString();
#endif
	

	std::wfstream f;

	f.open(MagicFile, std::fstream::in);

	if (f.is_open())
	{
		f.getline(local_data, MAX_GUID);
		f.close();
	}
	else
	{
		f.open(MagicFile, std::fstream::out);
		GUID local_guid;
		CoCreateGuid(&local_guid);
		swprintf_s(local_data, L"{%08x-%04x-%04x-%02x%02x-", local_guid.Data1, local_guid.Data2, local_guid.Data3, local_guid.Data4[0], local_guid.Data4[1]);

		for (int i = 0; i < 6; ++i)
		{
			swprintf_s(&local_data[25 + 2 * i], 4, L"%02x", local_guid.Data4[i + 2]);
		}

		swprintf_s(&local_data[25 + 2 * 6], 4, L"}\0");

		f << local_data;
		f.close();
	}

	m_localIdentifier = ref new Platform::String(local_data);
#endif
}

WCPToolkit::PasswordVaultManager::~PasswordVaultManager()
{

}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::SetValue(Platform::String^ key, Platform::String^ value, SecureStorageWrapper::ActionType type)
{
	Platform::String^ password = nullptr;
	SecureStorageWrapper::Error err = GetCurrentPassword(password);
	if (err != SecureStorageWrapper::E_SUCCESS)
	{
		return err;
	}
	
	PasswordVaultEntry entry;
	try
	{
		entry.GetFromString(password);
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_CORRUPTENTRY;
	}
	entry.m_map[key] = value;
	
	return RawSetValue(m_machineId, m_localIdentifier, entry.ToString());
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::GetValue(Platform::String^ key, Platform::String^& out_value)
{
	Platform::String^ password = nullptr;
	SecureStorageWrapper::Error err = GetCurrentPassword(password);
	if (err != SecureStorageWrapper::E_SUCCESS)
	{
		return err;
	}

	PasswordVaultEntry entry;
	try
	{
		entry.GetFromString(password);
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_CORRUPTENTRY;
	}
	out_value = entry.m_map[key];

	return SecureStorageWrapper::E_SUCCESS;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::ResetPasswordVault()
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		PasswordVaultEntry PVentry;
		PVentry.m_version = VERSION;
		auto entry = ref new Windows::Security::Credentials::PasswordCredential(m_machineId, m_localIdentifier, PVentry.ToString());
		vault->Add(entry);
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_TOOMANYENTRIES;
	}

	return SecureStorageWrapper::E_SUCCESS;
#endif

	return SecureStorageWrapper::E_ERROR_UNKNOWN;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::GetCurrentPassword(Platform::String^& current_password)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	Windows::Security::Credentials::PasswordCredential^ entry = nullptr;

	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		auto creds = vault->RetrieveAll();
		int sizeCreds = creds->Size;
		
		// search in all the credentials
		for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
		{
			Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
			if (c->Resource == m_machineId && c->UserName == m_localIdentifier)
			{
				c->RetrievePassword();
				current_password = c->Password;
				return SecureStorageWrapper::E_SUCCESS;
			}
		}
		
		// search the best match using the hardware id
		if (entry == nullptr)
		{
			for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
			{
				Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
				if (c->Resource == m_machineId)
				{
					c->RetrievePassword();
					entry = ref new Windows::Security::Credentials::PasswordCredential(m_machineId, m_localIdentifier, c->Password);
				}
			}
		}
		
		// search the best match using the local identifier
		if (entry == nullptr)
		{
			for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
			{
				Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
				if (c->UserName == m_localIdentifier)
				{
					c->RetrievePassword();
					entry = ref new Windows::Security::Credentials::PasswordCredential(m_machineId, m_localIdentifier, c->Password);
				}
			}
		}
		
		if (entry == nullptr)
		{
			// if no match was possible and no fallbacks, then add an entry to the password vault
			PasswordVaultEntry PVentry;
			PVentry.m_version = VERSION;
			current_password = PVentry.ToString();
			return SecureStorageWrapper::E_SUCCESS;
		}
		else
		{
			int sizeCreds = creds->Size;
			// delete all the partial possible matching from the password vault
			for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
			{
				Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
				if ((c->Resource == m_machineId && c->UserName != m_localIdentifier) || (c->Resource != m_machineId && c->UserName == m_localIdentifier))
				{
					vault->Remove(c);
				}
			}
			
			// add an entry with the new value
			entry->Resource = m_machineId;
			entry->UserName = m_localIdentifier;	
			current_password = entry->Password;
			
			vault->Add(entry);
			
			return SecureStorageWrapper::E_SUCCESS;
		}
	}
	catch (...)
	{
		if (entry == nullptr)
		{
			return SecureStorageWrapper::E_ERROR_UNKNOWN;
		}

		if (entry->Resource->Length() + entry->UserName->Length() + entry->Password->Length() > MAX_ENTRY_SIZE)
		{
			return SecureStorageWrapper::E_ERROR_NOTENOUGHSPACE;
		}

		return SecureStorageWrapper::E_ERROR_TOOMANYENTRIES;
	}

	return SecureStorageWrapper::E_SUCCESS;
#endif

	return SecureStorageWrapper::E_ERROR_UNKNOWN;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::RawSetValue(Platform::String^ resource, Platform::String^ user, Platform::String^ password)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		auto entry = ref new Windows::Security::Credentials::PasswordCredential(resource, user, password);
		
		vault->Add(entry);
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_UNKNOWN;
	}
	return SecureStorageWrapper::E_SUCCESS;
#endif

	return SecureStorageWrapper::E_ERROR_UNKNOWN;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::RawGetValue(Platform::String^ resource, Platform::String^ user, Platform::String^& out_password)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		auto creds = vault->RetrieveAll();

		int sizeCreds = creds->Size;
		for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
		{
			Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
			if (c->Resource == resource && c->UserName == user)
			{
				c->RetrievePassword();
				out_password = ref new Platform::String(c->Password->Data());
				return SecureStorageWrapper::E_SUCCESS;
			}
		}
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_UNKNOWN;
	}
#endif
	return SecureStorageWrapper::E_ERROR_NOENTRY;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::RawIsEntry(Platform::String^ resource, Platform::String^ user)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	Windows::Foundation::Collections::IVectorView<Windows::Security::Credentials::PasswordCredential^>^ creds;
	SecureStorageWrapper::Error err = SecureStorageWrapper::E_SUCCESS;

	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		auto creds = vault->RetrieveAll();

		int sizeCreds = creds->Size;
		for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
		{
			Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
			if (c->Resource == resource && c->UserName == user)
			{
				return SecureStorageWrapper::E_SUCCESS;
			}
		}
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_UNKNOWN;
	}
#endif
	return SecureStorageWrapper::E_ERROR_NOENTRY;
}

WCPToolkit::SecureStorageWrapper::Error WCPToolkit::PasswordVaultManager::RawRemoveEntry(Platform::String^ resource, Platform::String^ user)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	try
	{
		auto vault = ref new Windows::Security::Credentials::PasswordVault();
		auto creds = vault->RetrieveAll();
		int sizeCreds = creds->Size;
		for (int indexCreds = 0; indexCreds < sizeCreds; ++indexCreds)
		{
			Windows::Security::Credentials::PasswordCredential^ c = creds->GetAt(indexCreds);
			if (c->Resource == resource && c->UserName == user)
			{
				vault->Remove(c);
				return SecureStorageWrapper::E_SUCCESS;
			}
		}
	}
	catch (...)
	{
		return SecureStorageWrapper::E_ERROR_UNKNOWN;
	}
#endif

	return SecureStorageWrapper::E_ERROR_NOENTRY;
}