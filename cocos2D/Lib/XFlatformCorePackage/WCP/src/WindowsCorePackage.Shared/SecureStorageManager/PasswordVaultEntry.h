#pragma once

#include "../pch.h"

namespace WCPToolkit
{
	class PasswordVaultEntry
	{
	public:
		int m_version;
		std::map<Platform::String^, Platform::String^> m_map;

		Platform::String^ ToString();
		void GetFromString(Platform::String^ pass);
	};
};