#pragma once

namespace WCPToolkit
{
	template<typename T>
	class ISingleton
	{
	protected:
		inline explicit ISingleton<T>(){}
		inline ISingleton(const ISingleton&){}
		inline ISingleton& operator=(const ISingleton&){}
		virtual ~ISingleton(){}
	public:
		static void CreateInstance()
		{
			if (ms_pInstance == nullptr)
			{
				ms_pInstance = new T;
			}
		}
		static T* GetInstance()
		{
			CreateInstance();
			return ms_pInstance;
		}
		static void DestroyInstance()
		{
			if (ms_pInstance)
			{
				delete ms_pInstance;
				ms_pInstance = nullptr;
			}
		}

	protected:
		static T* ms_pInstance;
	};

	template<typename T>
	T* ISingleton<T>::ms_pInstance = nullptr;
};