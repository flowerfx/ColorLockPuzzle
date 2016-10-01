#ifndef _BOOST_MANAGER_H_
#define _BOOST_MANAGER_H_
#pragma once
#include "../LoadDataXML.h"
namespace Utility
{
	namespace Boost
	{
		struct BoostItem
		{
			RKString p_name;
			RKString p_DisplayName;
			//
			int p_costItem;
			int p_RemainItem;
			int p_NumberGain;
		public:
			BoostItem() {
				p_name = "";
				p_DisplayName = "";

				p_costItem = 0;
				p_RemainItem = 0;
				p_RemainItem = 0;
			}
		};

		class BoostManager
		{
		private:

			static BoostManager * m_Instance;

			std::map<RKString, BoostItem*>		p_listBoostItem;

		public:
			static BoostManager * GetInstance()
			{
				if (!m_Instance)
				{
					m_Instance = new BoostManager();
				}
				return m_Instance;
			}


			BoostManager();
			~BoostManager();

			void LoadXMLData();

			BoostItem* GetItem(RKString nameItem);

			void OnSetItemRemainAtName(RKString name, float item_remain);

		protected:
			void OnParseData(TiXmlDocument * p_ItemXML);

		};
	}
#define BoostMgr Boost::BoostManager::GetInstance()
}

#endif //_BOOST_MANAGER_H_