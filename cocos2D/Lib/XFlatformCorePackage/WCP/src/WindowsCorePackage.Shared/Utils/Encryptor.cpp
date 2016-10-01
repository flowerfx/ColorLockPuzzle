#include "pch.h"
#include "Encryptor.h"

namespace WCPToolkit
{
	namespace Utils
	{
		namespace Encryption
		{
			Platform::String^ Encryptor::Encrypt(Platform::String^ input, EncryptionType encription)
			{
				switch (encription)
				{
				case EncryptionType::EET_B64BASE:
					return EncryptB64(input, true);
				case EncryptionType::EET_AES:
					return EncryptAES(input, true);
				default:
					return "Unsupported Encryption Selected";
				}
			}
			Platform::String^ Encryptor::Decrypt(Platform::String^ input, EncryptionType encription)
			{
				switch (encription)
				{
				case EncryptionType::EET_B64BASE:
					return EncryptB64(input, false);
				case EncryptionType::EET_AES:
					return EncryptAES(input, false);
				default:
					return "Unsupported Encryption Selected";
				}
			}

			Platform::String^ Encryptor::EncryptB64(Platform::String^ input, bool bEncrypt)
			{
				Platform::String^ rez = L"";
				if (bEncrypt)
				{
					try
					{
						auto encData = Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(input, Windows::Security::Cryptography::BinaryStringEncoding::Utf8);
						rez = Windows::Security::Cryptography::CryptographicBuffer::EncodeToBase64String(encData);
					}
					catch (Platform::Exception^ e)
					{
						rez = "Exception in b64Base Encoding " + e->ToString();
					}
				}
				else
				{
					try
					{
						auto decData = Windows::Security::Cryptography::CryptographicBuffer::DecodeFromBase64String(input);
						rez = Windows::Security::Cryptography::CryptographicBuffer::ConvertBinaryToString(Windows::Security::Cryptography::BinaryStringEncoding::Utf8, decData);
					}
					catch (Platform::Exception^ e)
					{
						rez = "Exception in b64Base Decoding " + e->ToString();
					}
				}
				return rez;
			}

			Platform::Array<uint8>^ Encryptor::GetBytesB64(Platform::String^ input)
			{
				Platform::Array<uint8>^ rez;
				auto decData = Windows::Security::Cryptography::CryptographicBuffer::DecodeFromBase64String(input);
				Windows::Security::Cryptography::CryptographicBuffer::CopyToByteArray(decData, &rez);

				return rez;
			}

			Platform::String^ Encryptor::EncryptAES(Platform::String^ input, bool bEncrypt)
			{
				Platform::String^ rez = input;
				try
				{
					auto aesAlg = Windows::Security::Cryptography::Core::SymmetricKeyAlgorithmProvider::OpenAlgorithm(Windows::Security::Cryptography::Core::SymmetricAlgorithmNames::AesCbcPkcs7);

					Platform::String^ key = GetKey();
					Platform::Array<uint8>^ decodedKeyBytes = GetBytesB64(key);

					auto keyMaterial = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(decodedKeyBytes);
					auto theKey = aesAlg->CreateSymmetricKey(keyMaterial);
					auto decodedIVBytes = GetBytesB64(GetIV());
					auto iv = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(decodedIVBytes);

					if (bEncrypt)
					{
						auto encrpyMe = Windows::Security::Cryptography::CryptographicBuffer::ConvertStringToBinary(input, Windows::Security::Cryptography::BinaryStringEncoding::Utf8);

						auto encrypted = Windows::Security::Cryptography::Core::CryptographicEngine::Encrypt(theKey, encrpyMe, iv);

						rez = Windows::Security::Cryptography::CryptographicBuffer::EncodeToBase64String(encrypted);

					}
					else
					{
						auto decryptMe = Windows::Security::Cryptography::CryptographicBuffer::DecodeFromBase64String(input);

						auto decrypted = Windows::Security::Cryptography::Core::CryptographicEngine::Decrypt(theKey, decryptMe, iv);

						rez = Windows::Security::Cryptography::CryptographicBuffer::ConvertBinaryToString(Windows::Security::Cryptography::BinaryStringEncoding::Utf8, decrypted);
					}
				}
				catch (Platform::Exception^ e)
				{
					Platform::String^ msg = "Encryptor error : " + e->ToString();
					rez = msg;
				}
				return rez;
			}

			// respect the multiple of 4 length needed for b64
			Platform::String^ Encryptor::PadString(Platform::String^ input)
			{
				int len = input->Length() % 4;
				Platform::String^ rez = input;

				for (unsigned int i = 0; i < input->Length() + 4 - len; ++i)
				{
					rez += "=";
				}

				return rez;
			}

			//AES key
			Platform::String^ Encryptor::GetKey()
			{
				//Key returned in clear : GmA0FF9d004k9BPt167yNv==
				Platform::Array<uint8>^ aux = ref new Platform::Array<uint8>(24);
				unsigned char vector[] = {
					0, 38, 212, 239, 22, 0, 243, 43, 204, 0,
					4, 55, 206, 9, 14, 36, 189, 5, 1, 66,
					213, 40, 199, 0, 0
				};

				unsigned char start = 71;

				aux[0] = start;
				for (int i = 1; i < 24; i++)
				{
					aux[i] = aux[i - 1] + vector[i];
				}

				auto buff = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(aux);
				return Windows::Security::Cryptography::CryptographicBuffer::ConvertBinaryToString(Windows::Security::Cryptography::BinaryStringEncoding::Utf8, buff);
			}
			//AES IV
			Platform::String^ Encryptor::GetIV()
			{
				//IV returned in clear : OrdIpaV9jlrxPxdQRI0ud4==
				Platform::Array<uint8>^ aux = ref new Platform::Array<uint8>(24);
				unsigned char vector[] = {
					0, 35, 242, 229, 39, 241, 245, 227, 49, 2,
					6, 6, 216, 40, 236, 237, 1, 247, 231, 69,
					239, 208, 9, 0, 0
				};

				unsigned char start = 79;

				aux[0] = start;
				for (int i = 1; i < 24; i++)
				{
					aux[i] = aux[i - 1] + vector[i];

				}

				auto buff = Windows::Security::Cryptography::CryptographicBuffer::CreateFromByteArray(aux);
				return Windows::Security::Cryptography::CryptographicBuffer::ConvertBinaryToString(Windows::Security::Cryptography::BinaryStringEncoding::Utf8, buff);
			}

		}
	}
}