#include "pch.h"

#include "PasswordVaultManagerConfig.h"
#include "FileStorage.h"

namespace WCPToolkit
{
	namespace FileStorage
	{
		SecureStorageWrapper::Error SetValue(const std::string& key, const std::string& value, SecureStorageWrapper::ActionType type)
		{
			auto folder = Windows::Storage::ApplicationData::Current->LocalFolder;
			auto path = folder->Path;

			std::wstring filepath(path->Begin(), path->End());
			std::wstring local_key(key.begin(), key.end());

			filepath = filepath + L'\\' + STORAGE_FOLDER;

			CreateDirectoryW(filepath.c_str(), nullptr);

			filepath = filepath + L"\\" + local_key + L".dat";

			FILE* f;

			if (_wfopen_s(&f, filepath.c_str(), L"wb") != 0)
			{
				return SecureStorageWrapper::E_ERROR_UNKNOWN;
			}

			__int32 size = value.size();
			fwrite(&size, sizeof(size), 1, f);
			fwrite(value.c_str(), 1, size, f);

			fclose(f);

			return SecureStorageWrapper::E_SUCCESS;
		}

		SecureStorageWrapper::Error GetValue(const std::string& key, std::string& out_value)
		{
			auto folder = Windows::Storage::ApplicationData::Current->LocalFolder;
			auto path = folder->Path;

			std::wstring filepath(path->Begin(), path->End());
			std::wstring local_key(key.begin(), key.end());

			filepath = filepath + L'\\' + STORAGE_FOLDER + L"\\" + local_key + L".dat";

			FILE* f;

			if (_wfopen_s(&f, filepath.c_str(), L"rb") != 0)
			{
				return SecureStorageWrapper::E_ERROR_NOENTRY;
			}

			__int32 size;
			fread(&size, sizeof(size), 1, f);

			out_value.resize(size);
			fread((void*)(out_value.c_str()), 1, size, f);

			fclose(f);

			return SecureStorageWrapper::E_SUCCESS;
		}

		SecureStorageWrapper::Error RemoveKey(const std::string& key, SecureStorageWrapper::ActionType type)
		{
			auto folder = Windows::Storage::ApplicationData::Current->LocalFolder;
			auto path = folder->Path;

			std::wstring filepath(path->Begin(), path->End());
			std::wstring local_key(key.begin(), key.end());

			filepath = filepath + L'\\' + STORAGE_FOLDER + L"\\" + local_key + L".dat";

			_wremove(filepath.c_str());

			return SecureStorageWrapper::E_SUCCESS;
		}

		SecureStorageWrapper::Error ResetFileStorage()
		{
			auto folder = Windows::Storage::ApplicationData::Current->LocalFolder;
			auto path = folder->Path;

			std::wstring folderpath(path->Begin(), path->End());

			folderpath = folderpath + L'\\' + STORAGE_FOLDER;

			RemoveDirectoryW(folderpath.c_str());

			return SecureStorageWrapper::E_SUCCESS;
		}
	}
}