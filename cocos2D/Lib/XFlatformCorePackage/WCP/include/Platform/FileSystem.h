#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__


#include "PlatformBaseInterface/PlatformBase.h"


namespace platform
{
#if defined(WCP_DLL)
#define WINTK_EXPORTED __declspec(dllexport)
#else
#define WINTK_EXPORTED __declspec(dllimport)
#endif

    class WINTK_EXPORTED FileSystem : public FileSystemBase<FileSystem>
    {
		friend class FileSystemBase < FileSystem >;
	public:

		/** Is a wrapper over the remove function - http://www.cplusplus.com/reference/cstdio/rename/?kw=rename
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param oldFileName string containing the name of an existing file to be renamed and/or moved.Its value shall
		*        follow the file name specifications of the running environment and can include a path (if supported by the system).
		* @param newFileName string containing the new name for the file. Its value shall follow the file name 
		*        specifications of the running environment and can include a path (if supported by the system). 
		* @return 0 on success, on error -1 is returned and errno is set properly*/
		int Rename(const std::string& oldFileName, const std::string& newFileName) override;

		/** Remove is a wrapper over the remove function - http://www.cplusplus.com/reference/algorithm/remove/?kw=remove
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param fileName is the name of the file which is desired to remove
		* @return 0 on success, on error -1 is returned and errno is set properly*/
		int Remove(const std::string& fileName)  override;

		/** OpenFile is a wrapper over the fopen function - http://www.cplusplus.com/reference/cstdio/fopen/?kw=fopen
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param fileName is a C string containing the name of the file to be opened.
		* @param modeC string containing a file access mode
		* @return a pointer to a FILE on operation successfully, otherwise return a null pointer*/
		FILE* OpenFile(const std::string& fileName, const std::string& mode)  override;

		/** OpenFile is a wrapper over the fclose function - http://www.cplusplus.com/reference/cstdio/fclose/?kw=fclose
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		* @param file is a pointer to a FILE object that specifies the stream to be closed.
		* @return zero value if the stream is successfully closed, otherwise a EOF is returned.*/
		int CloseFile(FILE* file)  override;

		/** Use this function to return the size of the file
		* The filenames are relative to the 'GetStorageFilePath()'. Ex: if filename is 'test.txt' the full path
		* of this file will be for example '/sdcard/applicationName/etc/blabla/test.txt' .
		*@return On success, zero is returned. On error, -1 is returned.*/
		int GetFileSize(const std::string& fileName, off_t& size) override;

		/** Use this to get the platform dependent path location where files (related with this app) are stored.
		* @return a string containing the absolute path with / at the end . */
		std::string GetStorageFilePath() override;

		/** Use this to get the platform dependent path location where you can save some temporary files.
		* @return a string containing the absolute path with / at the end .*/
		std::string GetSaveFilePath() override;

		/** Use this to get the platform dependent path location where files (related with this app) are stored.
		* @return a wstring containing the absolute path with / at the end . */
		std::wstring GetStorageFilePathW();

		/** Use this to get the platform dependent path location where you can save some temporary files.
		* @return a wstring containing the absolute path with / at the end .*/
		std::wstring GetSaveFilePathW();

	private:
		FileSystem(){}
		
	#if (_MSC_VER < 1800) /// for VS before 2013 there is a bug that require public destructor for shared pointers.
	public:	
	#endif	
		~FileSystem(){}
    }; 
	
} // namespace platform

#endif