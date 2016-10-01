#include "pch.h"
#include "WinCryptography.h"

#ifdef _DEBUG
#define INTERNAL_ASSERT(cond) if(!(cond)){__debugbreak();}
#else
#define INTERNAL_ASSERT(cond)
#endif

static const char* s_base64EncodeTable = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void EncUrl_GetKeysFromChar(char nChar, char& hex1, char& hex2)
{
	hex1 = (nChar & 0xF0) >> 4;
	hex2 = (nChar & 0x0F);
	if (0 <= hex1 && hex1 <= 9) hex1 += '0';    //0 => 48 inascii
	if (10 <= hex1 && hex1 <= 15) hex1 += 'A' - 10; //A => 65 inascii
	if (0 <= hex2 && hex2 <= 9) hex2 += '0';
	if (10 <= hex2 && hex2 <= 15) hex2 += 'A' - 10;
}

bool EncodeUrlRFC3986(const std::string& source, std::string& dest)
{
	const char* c = source.c_str();
	size_t srcSize = source.size();

	for (size_t i = 0; i<srcSize; i++)
	{
		if ((48 <= c[i] && c[i] <= 57) ||//0-9 //digits
			(65 <= c[i] && c[i] <= 90) ||//abc...xyz //lowalpha
			(97 <= c[i] && c[i] <= 122) || //ABC...XYZ //highalpha
			(c[i] == '-' || c[i] == '.' || c[i] == '_' || c[i] == '~') //other
			)
		{
			dest.append(&c[i], 1);
		}
		else
		{
			dest.append("%");
			char hex1, hex2;
			EncUrl_GetKeysFromChar(c[i], hex1, hex2);

			dest.append(&hex1, 1);//converts char 255 to string "ff"
			dest.append(&hex2, 1);//converts char 255 to string "ff"
		}
	}

	return true;
}

char EncUrl_GetCharFromKeys(const char str[2])
{
	INTERNAL_ASSERT((str[0] >= '0' && str[0] <= '9') || (str[0] >= 'A' && str[0] <= 'F'));
	INTERNAL_ASSERT((str[1] >= '0' && str[1] <= '9') || (str[1] >= 'A' && str[1] <= 'F'));

	//Converts '0-9' to 0..9 and 'A..F' to 0xA..0xF

	const char hex1 = ('0' <= str[0] && str[0] <= '9') ? str[0] - '0' : str[0] - 0x37;
	const char hex2 = ('0' <= str[1] && str[1] <= '9') ? str[1] - '0' : str[1] - 0x37;

	return ((hex1 << 4) & 0xF0) + (hex2 & 0x0F);
}

bool DecodeUrl(const std::string& source, std::string& dest)
{
	const char* c = source.c_str();
	size_t srcSize = source.size();

	char decodedChar;

	for (size_t i = 0; i<srcSize; i++)
	{
		if (c[i] == '%')
		{
			decodedChar = EncUrl_GetCharFromKeys(&c[i + 1]);
			dest.append(&decodedChar, 1);
			i += 2;//encoded char takes 3 bytes
		}
		else
		{
			dest.append(&c[i], 1);
		}
	}

	return true;
}

bool IsInBase64Alphabet(char data)
{
	return  ((data == '+') || (data == '/') || (data >= '0' && data <= '9') || (data >= 'A' && data <= 'Z') || (data >= 'a' && data <= 'z'));
}

char Codec_Dec64_GetKeyFromChar(char nChar)
{
	INTERNAL_ASSERT(IsInBase64Alphabet(nChar));

	if (nChar == '/')
	{
		return 63;
	}
	else if (nChar == '+')
	{
		return 62;
	}
	else if (nChar < 58)
	{//48-57 is '0'-'9', index is 52-61
		return (char)(nChar + 4);
	}
	else if (nChar < 91)
	{//65-91 is 'A'-'Z', index is 0-25 
		return (char)(nChar - 65);
	}
	else
	{//97-122 is 'a'-'z', index is 26-51
		return (char)(nChar - 71);
	}
}

size_t Codec_DecodeBase64Relaxed(const char* base64Data, size_t dataSize, void* outData)
{
	if (outData == 0)
		return 0;

	size_t length = dataSize;
	const char* data = base64Data;
	for (; data[length - 1] == '='; --length);

	const size_t tail_length = length % 4;

	const size_t baseLength = length - tail_length; //decode by group of 4 chars

	unsigned char codedChar[4];
	unsigned char* decodedChar = static_cast<unsigned char*>(outData);

	size_t outIndex = 0;
	size_t i = 0;
	for (; i < baseLength; i += 4)
	{
		codedChar[0] = Codec_Dec64_GetKeyFromChar(data[i]);
		codedChar[1] = Codec_Dec64_GetKeyFromChar(data[i + 1]);
		codedChar[2] = Codec_Dec64_GetKeyFromChar(data[i + 2]);
		codedChar[3] = Codec_Dec64_GetKeyFromChar(data[i + 3]);

		decodedChar[outIndex++] = (codedChar[0] << 2) + ((codedChar[1] & 0x3f) >> 4);
		decodedChar[outIndex++] = (codedChar[1] << 4) + ((codedChar[2] & 0x3c) >> 2);
		decodedChar[outIndex++] = (codedChar[2] << 6) + (codedChar[3] & 0x3f);

	}

	if (tail_length > 0)
	{
		if (tail_length == 1) //should never happen ...
		{
			// Do nothing 6bit is not enough to decode

		}
		else if (tail_length == 2) //encoded %3 + 1 (only 8 bits valid)
		{
			codedChar[0] = Codec_Dec64_GetKeyFromChar(data[i]);
			codedChar[1] = Codec_Dec64_GetKeyFromChar(data[i + 1]);

			decodedChar[outIndex++] = (codedChar[0] << 2) + ((codedChar[1] & 0x3f) >> 4);
		}
		else //encoded %3 + 2  (only 16 bits valid)
		{
			codedChar[0] = Codec_Dec64_GetKeyFromChar(data[i]);
			codedChar[1] = Codec_Dec64_GetKeyFromChar(data[i + 1]);
			codedChar[2] = Codec_Dec64_GetKeyFromChar(data[i + 2]);

			decodedChar[outIndex++] = (codedChar[0] << 2) + ((codedChar[1] & 0x3f) >> 4);
			decodedChar[outIndex++] = (codedChar[1] << 4) + ((codedChar[2] & 0x3c) >> 2);
		}
	}

	return outIndex;
}

bool Codec_EncodeBase64(const unsigned char* src, size_t dataSize, std::string& dest)
{
	//We skip the line CR/LF after 76 bytes tp avoid a conversion problem in NSURLRequest
	if (src == 0 || dataSize == 0)
		return false;

	size_t baseLength = dataSize - (dataSize % 3); //encode by group of 3 chars

	char codedChar[4];
	for (size_t i = 0; i < baseLength; i += 3)
	{
		codedChar[0] = s_base64EncodeTable[src[i] >> 2];
		codedChar[1] = s_base64EncodeTable[((src[i] & 0x03) << 4) + (src[i + 1] >> 4)];
		codedChar[2] = s_base64EncodeTable[((src[i + 1] & 0x0f) << 2) + ((src[i + 2] & 0xC0) >> 6)];
		codedChar[3] = s_base64EncodeTable[src[i + 2] & 0x3f];
		dest.append(codedChar, 4);
	}

	if (baseLength != dataSize)
	{
		memset(codedChar, '=', 4); //Pad with =
		if (dataSize - baseLength == 1)
		{
			codedChar[0] = s_base64EncodeTable[src[baseLength] >> 2];
			codedChar[1] = s_base64EncodeTable[((src[baseLength] & 0x03) << 4)];
		}
		else //2
		{
			codedChar[0] = s_base64EncodeTable[src[baseLength] >> 2];
			codedChar[1] = s_base64EncodeTable[((src[baseLength] & 0x03) << 4) + (src[baseLength + 1] >> 4)];
			codedChar[2] = s_base64EncodeTable[((src[baseLength + 1] & 0x0f) << 2)];
		}
		dest.append(codedChar, 4);
	}

	return true;
}

void WIN_set_odd_parity_single(unsigned char& x)
{
	x = x & 0xFE;
	unsigned char aux = x ^ (x >> 4);
	aux = aux ^ (aux >> 2);
	aux = aux ^ (aux >> 1);
	x += 1 - (aux & 0x01);
}

void WIN_set_odd_parity(unsigned char *key)
{
	unsigned int i;

	for (i = 0; i < 8; i++)
		WIN_set_odd_parity_single(key[i]);
}

#if defined(OS_W8) || defined(OS_WP81) || defined(OS_W10)
std::string WinCryptography::Encrypt(const char * Key, const std::string & input)
{
	int blocks_number = (((int)input.size() % 8) == 0) ? ((int)input.size() / 8) : ((int)input.size() / 8 + 1);
	int block_size = 8;

	char * Msg = new char[blocks_number * block_size + 1];
	memset(Msg, 0, blocks_number * block_size + 1);
	memcpy(Msg, input.c_str(), (int)input.size());
	
	unsigned char Key2[8];
	
	memcpy(Key2, Key, 8);
	WIN_set_odd_parity(Key2);

	Platform::Array<unsigned char>^ array_encryptedData = ref new Platform::Array<unsigned char>((unsigned char*)Msg, blocks_number * block_size);
	Windows::Storage::Streams::IBuffer^ encryptedData = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(array_encryptedData);

	Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider^ algorithmProvider = Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::DesEcb);

	Platform::Array<unsigned char>^ array_key = ref new Platform::Array<unsigned char>((unsigned char*)Key2, 8);
	Windows::Storage::Streams::IBuffer^ key = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(array_key);

	Windows::Security::Cryptography::Core::CryptographicKey^ Criptkey = algorithmProvider->CreateSymmetricKey(key);

	Windows::Storage::Streams::IBuffer^ Decripted_HDIDFV = Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(Criptkey, encryptedData, nullptr);

	Platform::Array<unsigned char>^ destination;

	Windows::Security::Cryptography::CryptographicBuffer::CopyToByteArray(Decripted_HDIDFV, &destination);

	std::string encryptedHDIDFV((char*)(destination->Data), destination->Length);

	
	std::string tempEncoded;
	Codec_EncodeBase64((const unsigned char*)encryptedHDIDFV.c_str(), encryptedHDIDFV.length(), tempEncoded);

	std::string output;
	EncodeUrlRFC3986(tempEncoded, output);

	// Erase
	delete[] Msg;

	// Return
	return output;
}

std::string WinCryptography::Decrypt(const char * Key, const std::string & input)
{
	std::string aux;
	DecodeUrl(input, aux);

	char decodedCharArray[500];
	memset(decodedCharArray, 0, 500);
	Codec_DecodeBase64Relaxed(aux.c_str(), aux.length(), decodedCharArray);

	std::string aux2(decodedCharArray);
	std::string _input;
	DecodeUrl(aux2, _input);

	int blocks_number = (((int)_input.size() % 8) == 0) ? ((int)_input.size() / 8) : ((int)_input.size() / 8 + 1);
	int block_size = 8;

	char * Msg = new char[blocks_number * block_size + 1];
	memset(Msg, 0, blocks_number * block_size + 1);
	memcpy(Msg, _input.c_str(), (int)_input.size());

	unsigned char Key2[8];

	memcpy(Key2, Key, 8);
	WIN_set_odd_parity(Key2);

	Platform::Array<unsigned char>^ array_encryptedData = ref new Platform::Array<unsigned char>((unsigned char*)Msg, blocks_number * block_size);
	Windows::Storage::Streams::IBuffer^ encryptedData = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(array_encryptedData);
	
	Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider^ algorithmProvider = Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::DesEcb);
	
	Platform::Array<unsigned char>^ array_key = ref new Platform::Array<unsigned char>((unsigned char*)Key2, 8);
	Windows::Storage::Streams::IBuffer^ key = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(array_key);
	
	Windows::Security::Cryptography::Core::CryptographicKey^ Criptkey = algorithmProvider->CreateSymmetricKey(key);
	
	Windows::Storage::Streams::IBuffer^ Decripted_HDIDFV = Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(Criptkey, encryptedData, nullptr);
	
	Platform::Array<unsigned char>^ destination;
	
	Windows::Security::Cryptography::CryptographicBuffer::CopyToByteArray(Decripted_HDIDFV, &destination);
	
	std::string decryptedHDIDFV((char*)(destination->Data), destination->Length);
	
	// Erase
	delete[] Msg;

	return decryptedHDIDFV;
}
#endif