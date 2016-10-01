#include "pch.h"

#include "PasswordVaultManagerConfig.h"
#include "PasswordVaultEntry.h"
#include <iterator>

Platform::String^ WCPToolkit::PasswordVaultEntry::ToString()
{
	Platform::String^ pass = ref new Platform::String();

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	wchar_t buffer[MAX_ENTRY_SIZE];
	int map_size = 0;

	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		if (it->first->Length() > 0 && it->second->Length() > 0)
		{
			map_size++;
		}
	}

	swprintf_s(buffer, L"%02x", map_size);
	pass = pass + ref new Platform::String(buffer);

	for (auto it = m_map.begin(); it != m_map.end(); ++it)
	{
		if (it->first->Length() <= 0 || it->second->Length() <= 0)
		{
			continue;
		}

		swprintf_s(buffer, L"%04x", it->first->Length());
		pass = pass + ref new Platform::String(buffer);
		pass = pass + it->first;

		swprintf_s(buffer, L"%04x", it->second->Length());
		pass = pass + ref new Platform::String(buffer);
		pass = pass + it->second;
	}

	auto buff = Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(pass, Windows::Security::Cryptography::BinaryStringEncoding::Utf16LE);

	auto keyMaterial = Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(ENCRYPTION_KEY, Windows::Security::Cryptography::BinaryStringEncoding::Utf8);

	auto algorithmProvider = Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::DesEcbPkcs7);

	auto Criptkey = algorithmProvider->CreateSymmetricKey(keyMaterial);

	buff = Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(Criptkey, buff, nullptr);

	pass = Windows::Security::Cryptography::CryptographicBuffer::EncodeToBase64String(buff);

	swprintf_s(buffer, L"%02x", m_version);
	pass = ref new Platform::String(buffer) + pass;
#endif

	return pass;
}

void WCPToolkit::PasswordVaultEntry::GetFromString(Platform::String^ pass)
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	wchar_t buffer[MAX_ENTRY_SIZE];
	int offset;
	int length;
	int size;

	std::wstring version_buff(pass->Begin(), pass->Begin() + 2);

	stdext::checked_array_iterator<wchar_t *> chkd_buffer(buffer, MAX_ENTRY_SIZE);

	std::copy(version_buff.begin(), version_buff.begin() + 2, chkd_buffer);
	buffer[2] = 0;
	swscanf_s(buffer, L"%x", &m_version);

	auto password = ref new Platform::String(pass->Begin() + 2);

	auto str = Windows::Security::Cryptography::CryptographicBuffer::DecodeFromBase64String(password);

	auto keyMaterial = Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(ENCRYPTION_KEY, Windows::Security::Cryptography::BinaryStringEncoding::Utf8);

	auto algorithmProvider = Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::DesEcbPkcs7);

	auto Criptkey = algorithmProvider->CreateSymmetricKey(keyMaterial);

	str = Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(Criptkey, str, nullptr);

	pass = Windows::Security::Cryptography::CryptographicBuffer::ConvertBinaryToString(Windows::Security::Cryptography::BinaryStringEncoding::Utf16LE, str);

	std::wstring buff(pass->Data());

	std::copy(buff.begin() + 0, buff.begin() + 2, chkd_buffer);
	buffer[2] = 0;
	swscanf_s(buffer, L"%x", &size);

	offset = 2;

	for (int i = 0; i < size; ++i)
	{
		Platform::String^ key;
		Platform::String^ value;

		std::copy(buff.begin() + offset, buff.begin() + offset + 4, chkd_buffer);
		buffer[4] = 0;
		swscanf_s(buffer, L"%x", &length);

		std::copy(buff.begin() + offset + 4, buff.begin() + offset + 4 + length, chkd_buffer);
		buffer[length] = 0;
		key = ref new Platform::String(buffer);

		offset += 4 + length;

		std::copy(buff.begin() + offset, buff.begin() + offset + 4, chkd_buffer);
		buffer[4] = 0;
		swscanf_s(buffer, L"%x", &length);

		std::copy(buff.begin() + offset + 4, buff.begin() + offset + 4 + length, chkd_buffer);
		buffer[length] = 0;
		value = ref new Platform::String(buffer);

		offset += 4 + length;

		m_map[key] = value;
	}
#endif
}
