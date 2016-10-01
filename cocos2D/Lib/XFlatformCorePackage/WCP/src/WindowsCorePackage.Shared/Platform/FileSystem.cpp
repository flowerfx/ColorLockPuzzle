#include "pch.h"

#include "Platform/FileSystem.h"

#include <windows.h>

#include "Winshared.h"

namespace platform
{
	int FileSystem::Rename(const std::string& oldFileName, const std::string& newFileName)
	{
		std::wstring oldFileFullPath = GetStorageFilePathW() + WCPToolkit::Utils::Str2Wstr(oldFileName);
		if (oldFileFullPath.length() == 0)
			return -1;

		std::wstring newFileFullPath = GetStorageFilePathW() + WCPToolkit::Utils::Str2Wstr(newFileName);
		if (newFileFullPath.length() == 0)
			return -1;

		return _wrename(oldFileFullPath.c_str(), newFileFullPath.c_str());
	}

	int FileSystem::Remove(const std::string& fileName)
	{
		std::wstring fileNameFullPath = GetStorageFilePathW() + WCPToolkit::Utils::Str2Wstr(fileName);
		if (fileNameFullPath.length() == 0)
			return -1;

		return _wremove(fileNameFullPath.c_str());
	}

	FILE* FileSystem::OpenFile(const std::string& fileName, const std::string& mode)
	{
		std::wstring fileWithFullPath = GetStorageFilePathW() + WCPToolkit::Utils::Str2Wstr(fileName);
		if (fileWithFullPath.length() == 0)
			return nullptr;

		return _wfopen(fileWithFullPath.c_str(), WCPToolkit::Utils::Str2Wstr(mode).c_str());
	}

	int FileSystem::CloseFile(FILE* file)
	{
		return fclose(file);
	}

	int FileSystem::GetFileSize(const std::string& fileName, off_t& size)
	{
		std::wstring fileWithFullPath = GetStorageFilePathW() + WCPToolkit::Utils::Str2Wstr(fileName);
		if (fileWithFullPath.length() == 0)
		{
			return -1;
		}

		struct _stat auxBuf;
		int res = _wstat(fileWithFullPath.c_str(), &auxBuf);
		if (res == 0)
		{
			size = auxBuf.st_size;
		}

		return res;
	}

	std::string FileSystem::GetStorageFilePath()
	{
		return WCPToolkit::Utils::Wstr2Str(GetStorageFilePathW());
	}

	std::string FileSystem::GetSaveFilePath()
	{
		return WCPToolkit::Utils::Wstr2Str(GetSaveFilePathW());
	}

	std::wstring FileSystem::GetStorageFilePathW()
	{
		return WCPToolkit::Storage::GetAppLocalFolderPath() + L"\\";
	}

	std::wstring FileSystem::GetSaveFilePathW()
	{
		return WCPToolkit::Storage::GetAppLocalFolderPath() + L"\\";
	}
}
