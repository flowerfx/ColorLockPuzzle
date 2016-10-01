#pragma once

#include <string>


namespace AntiHack
{
	class AntiHackChecker
	{
	public:
		static AntiHackChecker* GetInstance();
		std::wstring GetTestString();
	private:
		static AntiHackChecker* s_Instance;
		static bool s_IsInstanceCreated;

		std::wstring m_BinaryPath;
		std::wstring m_TestString;
		std::wstring m_Publisher, m_PublisherID;
		bool m_IsStringConstructed;

		AntiHackChecker();

		int GetBinarySize();
	};
}