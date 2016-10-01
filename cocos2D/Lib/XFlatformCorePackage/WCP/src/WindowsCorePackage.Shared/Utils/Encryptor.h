namespace WCPToolkit
{
	namespace Utils
	{
		namespace Encryption
		{
			public enum class EncryptionType
			{
				EET_B64BASE,
				EET_AES,
				EET_TOTAL
			};

			public ref class Encryptor sealed
			{
			public:
				static Platform::String^ Encrypt(Platform::String^ input, EncryptionType encription);
				static Platform::String^ Decrypt(Platform::String^ input, EncryptionType encription);

				static Platform::String^ EncryptB64(Platform::String^ input, bool bEncrypt);

				static Platform::Array<uint8>^ GetBytesB64(Platform::String^ input);

				static Platform::String^ EncryptAES(Platform::String^ input, bool bEncrypt);

				// respect the multiple of 4 length needed for b64
				static Platform::String^ PadString(Platform::String^ input);

				//AES key
				static Platform::String^ GetKey();
				//AES IV
				static Platform::String^ GetIV();
			};
		}
	}
}