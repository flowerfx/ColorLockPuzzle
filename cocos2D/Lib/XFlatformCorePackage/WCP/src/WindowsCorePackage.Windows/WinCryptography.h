#pragma once

#include <string>

namespace WinCryptography
{
	std::string Encrypt(const char * Key, const std::string & input);
	std::string Decrypt(const char * Key, const std::string & input);
};
