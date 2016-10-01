#include "pch.h"
#include "AntiHack.h"

#include <fstream>
#include <sstream>
#include <time.h>

AntiHack::AntiHackChecker* AntiHack::AntiHackChecker::s_Instance = NULL;
bool AntiHack::AntiHackChecker::s_IsInstanceCreated = false;

wchar_t* s2ws(char* s, int size)
{
	int len;
	int slength = size + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s, slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s, slength, buf, len);
	return buf;
}

AntiHack::AntiHackChecker::AntiHackChecker()
{
	auto IL = Windows::ApplicationModel::Package::Current->InstalledLocation->Path->ToString();
	std::wstring path(IL->Begin(), IL->End());// (szArglist);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFileEx((path + L"\\*.exe").c_str(), FindExInfoStandard, &FindFileData, FindExSearchNameMatch, NULL, 0);

	if (hFind == INVALID_HANDLE_VALUE)
	{
#if defined(OS_W8)
		LPWSTR szArglist;
		szArglist = GetCommandLineW();

		std::wstring exe(szArglist);

		unsigned int first = exe.find(L"\"");
		unsigned int second = exe.find(L"\"", first + 1);
		m_BinaryPath = std::wstring(exe, first + 1, second - 1);
#endif
	}
	else
	{
		m_BinaryPath = path + L"\\" + FindFileData.cFileName;
		FindClose(hFind);
	}

	m_IsStringConstructed = false;

	Windows::ApplicationModel::Package^ package = Windows::ApplicationModel::Package::Current;
	Windows::ApplicationModel::PackageId^ packageId = package->Id;

	m_Publisher = std::wstring(packageId->Publisher->Data());
	m_PublisherID = std::wstring(packageId->PublisherId->Data());
}

int AntiHack::AntiHackChecker::GetBinarySize()
{
	int size = 0;
	std::basic_ifstream<unsigned char> binary(m_BinaryPath.c_str(), std::ios::binary | std::ios::in | std::ios::ate);

	if (!binary)
	{
		size = 0;
		return NULL;
	}

	size = (int)binary.tellg();

	binary.close();

	return size;
}

std::wstring AntiHack::AntiHackChecker::GetTestString()
{
	if (!m_IsStringConstructed)
	{
		int size = GetBinarySize();

		time_t t;
		time(&t);


		m_TestString = m_Publisher + L"|" + m_PublisherID + L"|" + std::to_wstring(size) + L"|" + std::to_wstring(t);
	}

	return m_TestString;
}

AntiHack::AntiHackChecker* AntiHack::AntiHackChecker::GetInstance()
{
	if (!s_IsInstanceCreated)
	{
		s_Instance = new AntiHackChecker();
		s_IsInstanceCreated = true;
	}

	return s_Instance;
}