#include "pch.h"

#include "WinShared.h"
#include "d3d11.h"
#pragma comment(lib, "d3d11.lib")
#include "dxgi.h"
#pragma comment(lib, "dxgi.lib")

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
#include "FileAgents.h"
#include "AntiHack.h"
#include <thread>
#include "WinCryptography.h"
#endif

#if defined(OS_WP81)
#include "client.h"
#endif

#ifdef OS_W10
#include "wrl/client.h"
#endif

#if defined(OS_WP8)
#include "Win8PhoneMacros.h"
#include "Win8PhoneCX.h"

namespace WCPToolkit
{
#if (WCP_TEST_SAMPLE == 1)
	//Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetCurrentHDIDFV()
	//{
	//	std::string sStr = WCPToolkit::Identifiers::GetCurrentHDIDFV(nullptr);
	//	std::wstring swStr(sStr.begin(), sStr.end());
	//	return ref new Platform::String(swStr.c_str());
	//}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetPublisherHostID()
	{
		std::string sStr = WCPToolkit::Identifiers::GetPublisherHostID();
		std::wstring swStr(sStr.begin(), sStr.end());
		return ref new Platform::String(swStr.c_str());
	}

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeTESTBRIDGEGetCustomDataID(Platform::String^ sIDstring)
	{
		std::wstring swStr(sIDstring->Data());

		//std::string sStr = WCPToolkit::Identifiers::GetCustomDataID(Wstr2Str(swStr));
		//swStr = str2wstr(sStr);
		return ref new Platform::String(swStr.c_str());
	}
#endif

	Platform::String^ WCPToolkitWinPhone80CppCallbacks::nativeGetAdvertisingID()
	{
		return GetAdvertisingID();
	}

}
#endif

namespace WCPToolkit
{
#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	const wchar_t* GAIA_WINDOWS_HDIDFV_RESSOURCE = L"GAIA_HDIDFV";
	const wchar_t* GAIA_WINDOWS_HDIDFV_USERNAME = L"GAIA_Identifier";
	const char* GAIA_WINDOWS_HDIDFV_KEY = "GAIA_HDIDFV";
	const char* GAIA_WINDOWS_HDIDFV_OLD_USER = "GAIA_Identifier";
#endif

#ifdef OS_WP8
	const std::string k_hdidfvFileStorageWP8 = "GTkI";
	const std::string k_hdidfvTmpFileStorageWP8 = "GTkITMP";
#endif

	//std::string Identifiers::GetCustomDataID(std::string sIDstring)
	//{
	//	std::string sStr = WCP_NOT_AVAILABLE;

	//	char * buffer;
	//	FILE* f = fopen("microsoft.system.package.metadata\\custom.data", "rb");

	//	if (f == NULL)
	//	{
	//		OutputDebugString(L"custom.data file not found!");

	//		std::wstring pathSave = Storage::GetAppLocalFolderPath();
	//		std::wstring nameFile = L"\\qa_custom.data";
	//		pathSave.append(nameFile);
	//		f = _wfopen(pathSave.c_str(), L"rb");
	//		if (f == NULL)
	//		{
	//			OutputDebugString(L"qa_custom.data file not found!");
	//			return sStr;
	//		}
	//	}
	//	fseek(f, 0, SEEK_END);
	//	long lSize = ftell(f);
	//	rewind(f);
	//	fclose(f);

	//	buffer = (char*)malloc(sizeof(char)*lSize + 1);
	//	if (buffer == NULL) 
	//	{ 
	//		OutputDebugString(L"Memory error");
	//		return sStr;
	//	}

	//	long result = fread(buffer, 1, lSize, f);
	//	buffer[lSize] = '\0';
	//	if (result != lSize) 
	//	{ 
	//		OutputDebugString(L"Reading error");
	//		return sStr;
	//	}

	//	std::string string_tmp(buffer);
	//	//std::string string_tmp = "<CustomData><IGP id = \"RXXX - IGP will be provided by Production team\" / >< / CustomData>";

	//	int nStartPos = string_tmp.find(sIDstring, 0);

	//	if (nStartPos != -1)
	//	{
	//		nStartPos = string_tmp.find("\"", nStartPos + 1);

	//		int nEndPos = -1;

	//		if (nStartPos != -1)
	//		{
	//			nEndPos = string_tmp.find("\"", nStartPos + 1);

	//			if (nEndPos != -1)
	//			{
	//				sStr = string_tmp.substr(nStartPos + 1, nEndPos - nStartPos - 1);
	//			}
	//			else
	//			{
	//				std::wstring swError = L"End of value for custom data param " + str2wstr(sIDstring) + L" not found!";
	//				OutputDebugString(swError.c_str());
	//			}
	//		}
	//		else
	//		{
	//			std::wstring swError = L"Begin of value for custom data param " + str2wstr(sIDstring) + L" not found!";
	//			OutputDebugString(swError.c_str());
	//		}
	//	}
	//	else
	//	{
	//		std::wstring swError = L"Custom data param " + str2wstr(sIDstring) + L" not found!";
	//		OutputDebugString(swError.c_str());
	//	}

	//	return sStr;
	//}

	std::string GenerateRandomGUID()
	{
		char local_data[40];
		GUID guid;
		CoCreateGuid(&guid);
		sprintf_s(local_data, "%08x-%04x-%04x-%02x%02x-", guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1]);

		for (int i = 0; i < 6; ++i)
		{
			sprintf_s(&local_data[24 + 2 * i], 4, "%02x", guid.Data4[i + 2]);
		}

		return std::string(local_data);
	}

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
	bool TryGetHDIDFV(std::string& encryptedHDIDFV)
	{
		//Look into old place
		WCPToolkit::SecureStorageWrapper::Error returnCode = WCPToolkit::SecureStorageWrapper::RawGetValue(GAIA_WINDOWS_HDIDFV_KEY, GAIA_WINDOWS_HDIDFV_OLD_USER, encryptedHDIDFV);
		if (returnCode == WCPToolkit::SecureStorageWrapper::Error::E_SUCCESS)
		{
			//Remove from old place
			WCPToolkit::SecureStorageWrapper::RawRemoveEntry(GAIA_WINDOWS_HDIDFV_KEY, GAIA_WINDOWS_HDIDFV_OLD_USER);
			//Set in new place
			WCPToolkit::SecureStorageWrapper::SetValue(GAIA_WINDOWS_HDIDFV_KEY, encryptedHDIDFV);

			return true;
		}

		returnCode = WCPToolkit::SecureStorageWrapper::GetValue(GAIA_WINDOWS_HDIDFV_KEY, encryptedHDIDFV);
		if (returnCode == WCPToolkit::SecureStorageWrapper::Error::E_SUCCESS)
			return true;
		else
			return false;
	}

	bool StoreHDIDFV(char* GameKey)
	{
		std::string GeneratedHDIDFV = GenerateRandomGUID();

		std::string EncryptedHDIDFV = WinCryptography::Encrypt(GameKey, GeneratedHDIDFV);

		WCPToolkit::SecureStorageWrapper::Error returnCode = WCPToolkit::SecureStorageWrapper::SetValue(GAIA_WINDOWS_HDIDFV_KEY, EncryptedHDIDFV);
		if (returnCode == WCPToolkit::SecureStorageWrapper::Error::E_SUCCESS)
			return true;
		else
			return false;
	}
#endif

//	std::string Identifiers::GetCurrentHDIDFV(char* GameKey)
//	{
//		static std::mutex GetCurrentHDIDFVMutex;
//		std::lock_guard<std::mutex> _lg(GetCurrentHDIDFVMutex);
//
//#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
//		std::string EncryptedHDIDFV = "";
//		bool result = TryGetHDIDFV(EncryptedHDIDFV);
//
//		if (!result)
//		{
//			/*We need to generate and store the HDIDFV*/
//			StoreHDIDFV(GameKey);
//			result = TryGetHDIDFV(EncryptedHDIDFV);
//		}
//
//	#ifdef _DEBUG
//			if (!result)
//			{
//				__debugbreak();
//			}
//	#endif
//
//		return WinCryptography::Decrypt(GameKey, EncryptedHDIDFV).data();
//#endif
//
//#ifdef OS_WP8
//		const std::string& k_hdidfvFileName = k_hdidfvFileStorageWP8;
//		const std::string& k_hdidfvFileNameTmp = k_hdidfvTmpFileStorageWP8;
//
//		std::wstring path = WCPToolkit::Storage::GetAppLocalFolderPath();
//
//		std::wstring hdidfvFile;
//		hdidfvFile.reserve(path.size() + k_hdidfvFileName.size() + 8);
//		hdidfvFile.append(path);
//		hdidfvFile.append(L"\\");
//		hdidfvFile.append(k_hdidfvFileName.begin(), k_hdidfvFileName.end());
//
//		std::wstring hdidfvTempFile;
//		hdidfvTempFile.reserve(path.size() + k_hdidfvFileNameTmp.size() + 8);
//		hdidfvTempFile.append(path);
//		hdidfvTempFile.append(L"\\");
//		hdidfvTempFile.append(k_hdidfvFileNameTmp.begin(), k_hdidfvFileNameTmp.end());
//
//		//remove the temp file if there is any
//		WCPToolkit::Storage::RemoveFile(hdidfvTempFile.c_str());
//
//		//the generated of the stored HDIDFV
//		std::string hdidfvId = "22222222-2222-2222-2222-222222222222";
//		bool failedToReadHDIDFV = true;
//
//		//try to read the stored HDIDFV
//
//		FILE* file = _wfopen(hdidfvFile.c_str(), L"rb");
//
//		if (file != NULL)
//		{
//			failedToReadHDIDFV = false;
//
//			//read the size of the HDIDFV
//			unsigned int size = 0;
//
//			//read the size of the HDIDFV
//			if (fread(&size, sizeof(size), 1, file) == 1)
//			{
//				//read the HDIDFV from the file
//				hdidfvId.resize(size);
//				if (fread(&hdidfvId[0], size, 1, file) == 1)
//				{
//					// the HDIDFV was read successfully
//					// return the answer after all the resources will be released
//				}
//				else
//				{
//					failedToReadHDIDFV = true;
//				}
//			}
//			else
//			{
//				failedToReadHDIDFV = true;
//			}
//
//			//close the file
//			fclose(file);
//
//			if (failedToReadHDIDFV == false)
//			{
//				return hdidfvId;
//			}
//			else
//			{
//				return "00000000-0000-0000-0000-000000000000";
//			}
//		}
//
//		//generate a new HDIDFV and delete the old one
//		WCPToolkit::Storage::RemoveFile(hdidfvFile.c_str());
//
//		FILE* tempFile = _wfopen(hdidfvTempFile.c_str(), L"wb");
//
//		//genereate a HDIDFV
//		hdidfvId = GenerateRandomGUID();
//		unsigned int size = hdidfvId.size();
//
//		size_t expectedWrittenObjs = 2;
//		size_t writtenObjs = 0;
//
//		writtenObjs += fwrite(&size, sizeof(size), 1, tempFile);
//		writtenObjs += fwrite(&hdidfvId[0], size, 1, tempFile);
//
//		//close the file
//		fclose(tempFile);
//
//		//check if the write ended successfully
//		if (writtenObjs != expectedWrittenObjs)
//		{
//			WCPToolkit::Storage::RemoveFile(hdidfvTempFile.c_str());
//			return "11111111-1111-1111-1111-111111111111";
//		}
//		else
//		{
//			WCPToolkit::Storage::RenameFile(hdidfvTempFile, hdidfvFile);
//		}
//
//		return hdidfvId;
//#endif
//	}

	std::string Identifiers::GetPublisherHostID()
	{
		std::string sStr = WCP_NOT_AVAILABLE;

#if defined(OS_WP8)
		auto id = Windows::Phone::System::Analytics::HostInformation::PublisherHostId;
		sStr = std::string(id->Begin(), id->End());
#endif

		return sStr;
	}

	std::string Identifiers::GetAdvertisingID()
	{
		std::string sStr = WCP_NOT_AVAILABLE;

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
		Platform::String^ sString;
		sString = Windows::System::UserProfile::AdvertisingManager::AdvertisingId;
		sStr = std::string(sString->Begin(), sString->End());
#elif defined(OS_WP8)
		Platform::String^ sString = NATIVE_CALL(nativeGetAdvertisingID)();
		sStr = std::string(sString->Begin(), sString->End());
#endif
		return sStr;
	}

	std::string Identifiers::GetHardwareID()
	{
		std::string sStr = WCP_NOT_AVAILABLE;

#if defined(OS_W10)
		Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation^ token = ref new Windows::Security::ExchangeActiveSyncProvisioning::EasClientDeviceInformation();
		if (token != nullptr)
		{
			// Create a HashAlgorithmProvider object.
			Windows::Security::Cryptography::Core::HashAlgorithmProvider^ objAlgProv = Windows::Security::Cryptography::Core::HashAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::HashAlgorithmNames::Md5);
			// Create a CryptographicHash object. This object can be reused to continually hash new messages.
			Windows::Security::Cryptography::Core::CryptographicHash^ objHash = objAlgProv->CreateHash();
			objHash->Append(Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(token->Id.ToString(), Windows::Security::Cryptography::BinaryStringEncoding::Utf8));
			Platform::String^ hash = Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(objHash->GetValueAndReset());
			sStr = std::string(hash->Begin(), hash->End());
		}

#elif defined(OS_W8) || defined(OS_WP81)
		Windows::System::Profile::HardwareToken^ token;
		token = Windows::System::Profile::HardwareIdentification::GetPackageSpecificToken(nullptr);

		// hardware id, signature, certificate IBuffer objects 
		// that can be accessed through properties.
		Windows::Storage::Streams::IBuffer^ hardwareId = token->Id;
		//Windows::Storage::Streams::IBuffer^ signature = token->Signature;
		//Windows::Storage::Streams::IBuffer^ certificate = token->Certificate;

		if (token != nullptr && token->Id != nullptr)
		{
			// Create a HashAlgorithmProvider object.
			auto objAlgProv = Windows::Security::Cryptography::Core::HashAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::HashAlgorithmNames::Md5);
			// Create a CryptographicHash object. This object can be reused to continually hash new messages.
			auto objHash = objAlgProv->CreateHash();
			objHash->Append(token->Id);
			auto hash = Windows::Security::Cryptography::CryptographicBuffer::EncodeToHexString(objHash->GetValueAndReset());
			sStr = std::string(hash->Begin(), hash->End());
		}
#endif

		return sStr;
	}

};