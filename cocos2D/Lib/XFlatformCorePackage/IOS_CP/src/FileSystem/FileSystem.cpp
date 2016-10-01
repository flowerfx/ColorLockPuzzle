#include "Platform/FileSystem.h"

#import <Foundation/NSArray.h>
#import <Foundation/Foundation.h>
#import <sstream>
#include <sys/stat.h>
#include <string>

namespace platform
{
    int FileSystem::Rename(const std::string& oldFileName, const std::string& newFileName)
    { 
        std::string oldFileFullPath = GetStorageFilePath() + oldFileName;
        if (oldFileFullPath.length() == 0)
            return -1;

        std::string newFileFullPath = GetStorageFilePath() + newFileName;
        if (newFileFullPath.length() == 0)
            return -1;

        return rename(oldFileFullPath.c_str(), newFileFullPath.c_str());
    }

    int FileSystem::Remove(const std::string& fileName)
    { 
        std::string fileNameFullPath = GetStorageFilePath() + fileName;
        if (fileNameFullPath.length() == 0)
            return -1;

        return remove(fileNameFullPath.c_str());
    }

    FILE* FileSystem::OpenFile(const std::string& fileName, const std::string& mode)
    { 
        std::string fileWithFullPath = GetStorageFilePath() + fileName;
        if (fileWithFullPath.length() == 0)
            return nullptr;

        return fopen(fileWithFullPath.c_str(), mode.c_str());
    }

    int FileSystem::CloseFile(FILE* file)
    { 
        return fclose(file);
    }

    int FileSystem::GetFileSize(const std::string& fileName, off_t& size)
    { 
        std::string fileWithFullPath = GetStorageFilePath() + fileName;
        if (fileWithFullPath.length() == 0)
        {
            return -1;
        }

        struct stat buf;
		int res = stat(fileWithFullPath.c_str(), &buf);
		if (res == 0)	//operation successfull
		{
			size = buf.st_size;
		}

		return res;
    }

    std::string FileSystem::GetStorageFilePath()
    { 
		const char*     mySavePath;
		NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES); 
		NSString *documentsDirectory = [paths objectAtIndex:0];
		mySavePath = [documentsDirectory UTF8String];
		
		std::stringstream ss;
		ss << mySavePath << "/";
		[pool release];
		return ss.str();
    }

    std::string FileSystem::GetSaveFilePath()
    {
		return GetStorageFilePath();
    }
}
