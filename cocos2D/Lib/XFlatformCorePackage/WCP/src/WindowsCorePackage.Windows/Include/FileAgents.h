#pragma once
#include "pch.h"

class OpenFileAgent : public concurrency::agent
{
public:
	explicit OpenFileAgent(const wchar_t* path) : m_OutputFile(nullptr), m_Success(false)
	{ 
		std::wstring wPath = std::wstring(path);

		std::replace(wPath.begin(), wPath.end(), L'/', L'\\');

		m_Path = ref new Platform::String(wPath.c_str());
	}
protected:
	void run()
	{
		try
		{
			auto process = Windows::Storage::StorageFile::GetFileFromPathAsync( m_Path);

			process->Completed = ref new Windows::Foundation::AsyncOperationCompletedHandler<Windows::Storage::StorageFile^>(
				[this] (Windows::Foundation::IAsyncOperation<Windows::Storage::StorageFile^>^ operation, Windows::Foundation::AsyncStatus asyncStatus) {
					
					if( operation->Status == Windows::Foundation::AsyncStatus::Completed )
					{ 
						this->m_OutputFile = operation->GetResults();

						this->m_Success = true; 
					}
					else
					{
						this->m_OutputFile = nullptr;
					}
					this->done();
				}
			);
			
		}
		catch(std::exception& e)
		{
			WCPToolkit::Utils::Log(WCPToolkit::Utils::Str2Wstr(e.what()));
			this->done();
		}
	}
private:
	Windows::Storage::StorageFile^	m_OutputFile;
	Platform::String^			m_Path;
	Platform::Boolean			m_Success;

public:
	Windows::Storage::StorageFile^ GetFile(){ return m_OutputFile;};

	Platform::Boolean Success()
	{
		return m_Success;
	}
};

class DeleteFileAgent : public concurrency::agent
{
public:
	explicit DeleteFileAgent(const wchar_t* path) : m_Success(false), m_Path(path){ }
protected:
	void run()
	{
		try
		{
			OpenFileAgent ag(m_Path);
			ag.start();
			concurrency::agent::wait(&ag);

			Platform::Boolean success = ag.Success();

			if ( success == true ){
				auto process = ag.GetFile()->DeleteAsync();

				process->Completed = ref new Windows::Foundation::AsyncActionCompletedHandler(
					[this] (Windows::Foundation::IAsyncAction^ action, Windows::Foundation::AsyncStatus asyncStatus2) {
						if( action->Status == Windows::Foundation::AsyncStatus::Completed )
						{
							m_Success = true;
						}
						else
						{

						}
						this->done();
					}
				);
			}
			else this->done();
			
		}
		catch(std::exception& e)
		{
			std::string s(e.what());
			std::wstring w(s.begin(), s.end());
			this->done();
		}
	}
private:
	const wchar_t*	m_Path;
	Platform::Boolean	m_Success;

public:
	Platform::Boolean Success()
	{
		return m_Success;
	}
};

class ReplaceFileAgent : public concurrency::agent
{
public:
	explicit ReplaceFileAgent(Windows::Storage::StorageFile^ file1, Windows::Storage::StorageFile^ file2) : m_Success(false){ m_File1 = file1; m_File2 = file2; }
protected:
	void run()
	{
		try
		{
			auto process = m_File1->RenameAsync( m_File2->DisplayName, Windows::Storage::NameCollisionOption::ReplaceExisting );

			process->Completed = ref new Windows::Foundation::AsyncActionCompletedHandler(
				[this] (Windows::Foundation::IAsyncAction^ action, Windows::Foundation::AsyncStatus asyncStatus2) {
					if( action->Status == Windows::Foundation::AsyncStatus::Completed )
					{
						m_Success = true;
					}
					else
					{

					}
					this->done();
				}
			);
			
		}
		catch(std::exception& e)
		{
			std::string s(e.what());
			std::wstring w(s.begin(), s.end());
			this->done();
		}
	}
private:
	Windows::Storage::StorageFile^	m_File1;
	Windows::Storage::StorageFile^	m_File2;
	Platform::Boolean			m_Success;

public:
	Platform::Boolean Success()
	{
		return m_Success;
	}
};

class MoveFileAgent : public concurrency::agent
{
public:
	explicit MoveFileAgent(Windows::Storage::StorageFile^ file, const wchar_t* name) : m_Success(false){ m_File = file; m_Name = ref new Platform::String(name);  }
protected:
	void run()
	{
		try
		{
			auto process = m_File->RenameAsync( m_Name, Windows::Storage::NameCollisionOption::ReplaceExisting );

			process->Completed = ref new Windows::Foundation::AsyncActionCompletedHandler(
				[this] (Windows::Foundation::IAsyncAction^ action, Windows::Foundation::AsyncStatus asyncStatus2) {
					if( action->Status == Windows::Foundation::AsyncStatus::Completed )
					{
						m_Success = true;
					}
					else
					{

					}
					this->done();
				}
			);
			
		}
		catch(std::exception& e)
		{
			std::string s(e.what());
			std::wstring w(s.begin(), s.end());
			this->done();
		}
	}
private:
	Windows::Storage::StorageFile^	m_File;
	Platform::String^			m_Name;
	Platform::Boolean			m_Success;

public:
	Platform::Boolean Success()
	{
		return m_Success;
	}
};
