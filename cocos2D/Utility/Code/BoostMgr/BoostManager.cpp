#include "BoostManager.h"
#include "../FileManager.h"
#include "../DisplayView.h"

namespace Utility
{
	namespace Boost
	{

		BoostManager * BoostManager::m_Instance = NULL;

		BoostManager::BoostManager()
		{
			p_listBoostItem.clear();
		}
		BoostManager::~BoostManager()
		{
			p_listBoostItem.clear();
		}
		
		void BoostManager::LoadXMLData()
		{

			TiXmlDocument * p_ItemXML = new TiXmlDocument();
			bool IsUseArk = false;
#if USE_ARK_ARCHIVE
			IsUseArk = true;
			if (IsUseArk)
			{
				tinyxml2::XMLError error = p_ItemXML->LoadFileArk(std::string(XML_DIR) + std::string(ITEM_XML));
				if (error == tinyxml2::XMLError::XML_NO_ERROR)
				{
					OnParseData(p_ItemXML);
				}
				else
				{
					IsUseArk = false;
				}
			}
#endif
			if (!IsUseArk)
			{
				FILE * f = File::GetFile((std::string(XML_DIR) + std::string(ITEM_XML)).c_str(), "rb");
				if (f)
				{
					p_ItemXML->LoadFile(f);
					OnParseData(p_ItemXML);

					fclose(f);
				}
			}

			delete p_ItemXML;
			p_ItemXML = NULL;
		}


		void BoostManager::OnParseData(TiXmlDocument * p_ItemXML)
		{
			auto root = p_ItemXML->FirstChildElement("GAME");
			if (!root)
			{
				CCASSERT(false, "root data not found !");
				return;
			}

			auto ListBoostItem = root->FirstChildElement("BoostItem");
			if (ListBoostItem)
			{
				int global_cost = 0;
				int global_number = 3;

				auto global_item = root->FirstChildElement("BoostItem");
				if (global_item)
				{
					auto str__ = global_item->Attribute("cost");
					if (str__)
					{
						global_cost = atoi(str__);
					}
					str__ = global_item->Attribute("number");
					if (str__)
					{
						global_number = atoi(str__);
					}
				}

				auto detail = ListBoostItem->FirstChildElement("Item");
				do
				{
					if (detail)
					{
						auto item = new BoostItem();
						RKString id_items = detail->Attribute("id");
						RKString name = detail->Attribute("name");
						item->p_name = name;
						//display name
						auto child_element_ = detail->FirstChildElement("displayName");
						if (child_element_)
						{
							item->p_DisplayName = child_element_->Attribute("value");
						}
						// cost item
						child_element_ = detail->FirstChildElement("cost_item");
						if (child_element_)
						{
							item->p_costItem = atoi(child_element_->Attribute("value"));
						}
						else
						{
							item->p_costItem = global_cost;
						}
						//number add
						child_element_ = detail->FirstChildElement("number_add");
						if (child_element_)
						{
							item->p_NumberGain = atoi(child_element_->Attribute("value"));
						}
						else
						{
							item->p_NumberGain = global_number;
						}
						// decsriptiom
						

						//remain
						float data_at_name = File::SaveMgr->GetDataSave()->GetDataByName(name);
						item->p_RemainItem = data_at_name;
						


						p_listBoostItem.insert(std::pair<RKString, BoostItem*>(id_items + "#" + name, item));


						detail = detail->NextSiblingElement();
					}

				} while (detail);
			}
		}

		BoostItem* BoostManager::GetItem(RKString nameItem)
		{
			for (auto it = p_listBoostItem.begin(); it != p_listBoostItem.end(); it++)
			{
				if (it->first.Contains(nameItem))
				{
					return it->second;
				}
			}
			return NULL;
		}

		
		void BoostManager::OnSetItemRemainAtName(RKString name, float item_remain)
		{
			
			for (auto it = p_listBoostItem.begin(); it != p_listBoostItem.end(); it++)
			{
				if (it->first.Contains(name))
				{
					it->second->p_RemainItem = (int)item_remain;

					//push to save
					File::SaveMgr->GetDataSave()->SetDataAtName(name, item_remain);

					return;
				}
			}
			
		}
	}
}