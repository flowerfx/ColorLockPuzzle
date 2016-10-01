#include "LoadDataXML.h"
#include "DisplayView.h"
std::vector<std::string> split(const std::string& s, char seperator)
{
	std::vector<std::string> output;

	std::string::size_type prev_pos = 0, pos = 0;

	while ((pos = s.find(seperator, pos)) != std::string::npos)
	{
		std::string substring(s.substr(prev_pos, pos - prev_pos));

		output.push_back(substring);

		prev_pos = ++pos;
	}

	output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word

	return output;
}

namespace Utility
{
	namespace xml
	{
		LoadDataXML *		 LoadDataXML::p_Instance = NULL;
		LoadDataXML::LoadDataXML()
		{
			p_HProfile = HARDWARD_PROFILE::HARDWARE_HIGH;
            
            p_texture_packer_xml = nullptr;

		}

		LoadDataXML::~LoadDataXML()
		{
			p_list_font.clear();
			p_listAbstractBasicObject.clear();
			p_listMaterial.clear();

			p_listWideObject.clear();
            
            if(p_texture_packer_xml)
            {
                delete p_texture_packer_xml;
                p_texture_packer_xml = nullptr;
            }

		}

		void LoadDataXML::OnLoadXMLData(STATE_LOAD_XML state_load)
		{
			bool p_IsUseArkXML = false;
#ifdef USE_ARK_ARCHIVE
			p_IsUseArkXML = true;
#endif
			TiXmlDocument * p_ObjectXML = NULL;
			p_ObjectXML = new TiXmlDocument();

			std::string TargetXML = "";
			switch (state_load)
			{
			case Utility::xml::LOAD_TEXTURE_XML:
				TargetXML = std::string(TEXTURE_XML);
				break;
			case Utility::xml::LOAD_UI_DEC_XML:
				TargetXML = std::string(UI_XML);
				break;
			case Utility::xml::LOAD_UI_WIDGET_XML:
				TargetXML = std::string(UI_WIDGET_XML);
				break;
			case Utility::xml::LOAD_BG_DEC_XML:
				TargetXML = std::string(BG_DEC_XML);
				break;
			case Utility::xml::LOAD_BASIC_DEC_XML:
				TargetXML = std::string(OBJECT_DEC_XML);
				break;
			case Utility::xml::LOAD_MATERIAL_DEC_XML:
				TargetXML = std::string(MATERIAL_DEC_XML);
				break;
			case Utility::xml::LOAD_SOUND_DEC_XML:
				TargetXML = std::string(SOUND_DEC_XML);
				break;
			case Utility::xml::LOAD_EFFECT_DEC_XML:
				TargetXML = std::string(EFFECT_DEC_XML);
				break;
			case Utility::xml::LOAD_W_OBJECT_DEC_XML:
				TargetXML = std::string(W_OBJECT_DEC_XML);
				break;
			case Utility::xml::LOAD_SCENE_3D_DEC_XML:
				TargetXML = std::string(SCENE_3D_DEC_XML);
				break;
			 case Utility::xml::LOAD_TEXTURE_PACKER_DEC_XML:
                TargetXML = std::string(TEXTURE_PACKER_XML);
                break;	
			default:
				break;
			}

#ifdef USE_ARK_ARCHIVE
			if (p_IsUseArkXML)
			{
				tinyxml2::XMLError error = p_ObjectXML->LoadFileArk(std::string(XML_DIR) + TargetXML);

				if (error == tinyxml2::XMLError::XML_NO_ERROR)
				{
					switch (state_load)
					{
					case Utility::xml::LOAD_TEXTURE_XML:
						OnLoadTextureDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_UI_DEC_XML:
						OnLoadUIDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_UI_WIDGET_XML:
						OnLoadUIWidgetDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_BG_DEC_XML:
						OnLoadBackgroundDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_BASIC_DEC_XML:
						OnLoadListObjectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_MATERIAL_DEC_XML:
						OnLoadMaterialDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_SOUND_DEC_XML:
						OnLoadListSoundDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_EFFECT_DEC_XML:
						OnLoadListEffectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_W_OBJECT_DEC_XML:
						OnLoadListWideObjectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_SCENE_3D_DEC_XML:
						OnLoadListScene3DXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_TEXTURE_PACKER_DEC_XML:
                        OnLoadTexurePackerDecXML(p_ObjectXML);
                        break;	
					default:
						break;
					}
				}
				else
				{
					p_IsUseArkXML = false;
				}
			}
#endif
			if (!p_IsUseArkXML)
			{
				FILE * f = File::GetFile((std::string(XML_DIR) + TargetXML).c_str(), "rb");
				if (f)
				{
					p_ObjectXML->LoadFile(f);
					switch (state_load)
					{
					case Utility::xml::LOAD_TEXTURE_XML:
						OnLoadTextureDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_UI_DEC_XML:
						OnLoadUIDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_UI_WIDGET_XML:
						OnLoadUIWidgetDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_BG_DEC_XML:
						OnLoadBackgroundDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_BASIC_DEC_XML:
						OnLoadListObjectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_MATERIAL_DEC_XML:
						OnLoadMaterialDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_SOUND_DEC_XML:
						OnLoadListSoundDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_EFFECT_DEC_XML:
						OnLoadListEffectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_W_OBJECT_DEC_XML:
						OnLoadListWideObjectDecXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_SCENE_3D_DEC_XML:
						OnLoadListScene3DXML(p_ObjectXML);
						break;
					case Utility::xml::LOAD_TEXTURE_PACKER_DEC_XML:
                        OnLoadTexurePackerDecXML(p_ObjectXML);
                        break;	
					default:
						break;
					}
					fclose(f);
				}
				else
				{
					PASSERT2(false, "XML file cannot be read or found !");
				}
			}

			delete p_ObjectXML;
			p_ObjectXML = NULL;

		}

		HARDWARD_PROFILE LoadDataXML::GetHarwardProfile()
		{
			return p_HProfile;
		}

		/*
		-----------------------------------------------------
		*/

		void LoadDataXML::InsertTextureXML(RKString str, AtlasTexture * te)
		{
			p_ListTextureXML.insert(std::pair<RKString, AtlasTexture*>(str, te));
		}
        
        void LoadDataXML::OnLoadTexurePackerDecXML(TiXmlDocument * objectXMl)
        {
            auto Root = objectXMl->FirstChildElement("GAME");
            if (!Root)
            {
                PASSERT2(Root == nullptr, "root must not be null! ");
                return;
            }
            
            Vec2 screen_size = GetGameSize();
            
            auto rootTexture = Root->FirstChildElement("texture");
            if (rootTexture)
            {
                auto textPack = rootTexture->FirstChildElement("texture_pack");
                do
                {
                    if (textPack)
                    {
                        auto size_fit_design = textPack->Attribute("fit_size");
                        auto range_size_x = textPack->Attribute("range_size_x");
                        
                        Vec2 _rg_size_x = ConvertToVector2(range_size_x);
                        
                        bool IsFitForThisDesign = false;
                        
                        //check the screen in the range of the design size
                        if(screen_size.x >=_rg_size_x.x && (screen_size.x <= _rg_size_x.y || _rg_size_x.y <= -1)
                            )
                        {
                            IsFitForThisDesign = true;
                        }
                        
                        if(IsFitForThisDesign)
                        {
                            
                            p_texture_packer_xml = new TextPacker();
                            
                            auto tex = textPack->FirstChildElement("tex");
                            do
                            {
                                if(tex)
                                {
									RKString name_tex = tex->Attribute("name");
									std::vector<TextPackerDetail> list_text_path;
									auto tex_path = tex->FirstChildElement("path");
									do
									{
										if (tex_path)
										{
											RKString path_tex = tex_path->Attribute("texture");
											RKString plist_tex = tex_path->Attribute("plist");
											list_text_path.push_back(TextPackerDetail(path_tex, plist_tex));
											tex_path = tex_path->NextSiblingElement();
										}
									} while (tex_path);
                                    
                                    p_texture_packer_xml->InsertTexPack(name_tex, list_text_path);
                                    p_texture_packer_xml->SizeFit = ConvertToVector2(size_fit_design);
                            
                                    tex = tex->NextSiblingElement("tex");
                                }
                                
                            }while(tex);
                            
                            auto root_font = textPack->FirstChildElement("font");
                            if (root_font)
                            {
                                
                                auto font_detail = root_font->FirstChildElement("fontdetail");
                                do
                                {
                                    if (font_detail)
                                    {
                                        FontDec * font_dec = new FontDec();
                                        font_dec->idx = atoi(font_detail->Attribute("id"));
                                        font_dec->path = font_detail->Attribute("namepath");
                                        RKString name_font = font_detail->Attribute("name");
                                        //
                                        font_dec->name = name_font;
                                        //
                                        auto default_value = font_detail->FirstChildElement("default");
                                        if (default_value)
                                        {
                                            font_dec->d_size = atof(default_value->Attribute("size"));
                                            font_dec->d_width_border = atof(default_value->Attribute("borderwdx"));
                                            RKString color = default_value->Attribute("color");
                                            
                                            auto val_slipt = color.Split("_");
                                            font_dec->d_color[0] = atoi(val_slipt[0].GetString());
                                            font_dec->d_color[1] = atoi(val_slipt[1].GetString());
                                            font_dec->d_color[2] = atoi(val_slipt[2].GetString());
                                            font_dec->d_color[3] = atoi(val_slipt[3].GetString());
                                            
											if (default_value->Attribute("colorborder"))
											{
												color = default_value->Attribute("colorborder");

												val_slipt = color.Split("_");
												font_dec->d_color_border[0] = atoi(val_slipt[0].GetString());
												font_dec->d_color_border[1] = atoi(val_slipt[1].GetString());
												font_dec->d_color_border[2] = atoi(val_slipt[2].GetString());
												font_dec->d_color_border[3] = atoi(val_slipt[3].GetString());
											}
											else
											{
												font_dec->d_color_border[0] = 255;
												font_dec->d_color_border[1] = 255;
												font_dec->d_color_border[2] = 255;
												font_dec->d_color_border[3] = 255;
											}
                                            
                                            font_dec->IsUnicode = (atoi(default_value->Attribute("unicode")) != 0);
                                        }
                                        
                                        InsertFontToList(name_font, font_dec);

										font_detail = font_detail->NextSiblingElement();
                                    }
                                   
                                } while (font_detail);
                                
                            }
                            
                            break;
                        }

                        textPack = textPack->NextSiblingElement();
                    }
                }while(textPack);
                
            }
            else
            {
                PASSERT2(rootTexture == nullptr, "rootTexture must not be null! ");
            }
        }
        
		void LoadDataXML::OnLoadTextureDecXML(TiXmlDocument * p_ObjectXML)
		{
			auto Root = p_ObjectXML->FirstChildElement("GAME");
			if (!Root)
				return;

			auto rootTexture = Root->FirstChildElement("texture");
			if (rootTexture)
			{
				auto texDetail = rootTexture->FirstChildElement("textureDetail");
				do
				{
					if (texDetail)
					{

						RKString extension = "";
#if defined OS_IOS
						extension = texDetail->Attribute("ios");
#else
						extension = texDetail->Attribute("win32");
#endif
						auto path = texDetail->Attribute("namepath") + extension;

						RKString nameTexture = texDetail->Attribute("name");

						AtlasTexture * new_texture = new AtlasTexture();
						new_texture->NameImage = nameTexture;
						new_texture->NamePath = path;

						auto t_sprite = texDetail->FirstChildElement("sprite");
						do
						{
							if (t_sprite)
							{
								AtlasChild  child;
								//
								child.nameSprite = t_sprite->Attribute("name");
								RKString rect = t_sprite->Attribute("rect");
								auto rect_detail = rect.Split("-");
								child.Position = Vec2(atof(rect_detail.GetAt(0).GetString()), atof(rect_detail.GetAt(1).GetString()));
								child.Size = Vec2(atof(rect_detail.GetAt(2).GetString()), atof(rect_detail.GetAt(3).GetString()));
								//
								new_texture->InsertAtlasChild(child.nameSprite, child);
								//
								t_sprite = t_sprite->NextSiblingElement();
							}

						} while (t_sprite);

						InsertTextureXML(nameTexture, new_texture);

						texDetail = texDetail->NextSiblingElement();
					}
				} while (texDetail);

			}

			rootTexture = Root->FirstChildElement("font");
			if (rootTexture)
			{

				auto font_detail = rootTexture->FirstChildElement("fontdetail");
				do
				{
					if (font_detail)
					{
						FontDec * font_dec = new FontDec();
						font_dec->idx = atoi(font_detail->Attribute("id"));
						font_dec->path = font_detail->Attribute("namepath");
						RKString name_font = font_detail->Attribute("name");
						//
						font_dec->name = name_font;
						//
						auto default_value = font_detail->FirstChildElement("default");
						if (default_value)
						{
							font_dec->d_size = atof(default_value->Attribute("size"));
							font_dec->d_width_border = atof(default_value->Attribute("borderwdx"));
							RKString color = default_value->Attribute("color");

							auto val_slipt = color.Split("_");
							font_dec->d_color[0] = atoi(val_slipt[0].GetString());
							font_dec->d_color[1] = atoi(val_slipt[1].GetString());
							font_dec->d_color[2] = atoi(val_slipt[2].GetString());
							font_dec->d_color[3] = atoi(val_slipt[3].GetString());

							color = default_value->Attribute("colorborder");

							val_slipt = color.Split("_");
							font_dec->d_color_border[0] = atoi(val_slipt[0].GetString());
							font_dec->d_color_border[1] = atoi(val_slipt[1].GetString());
							font_dec->d_color_border[2] = atoi(val_slipt[2].GetString());
							font_dec->d_color_border[3] = atoi(val_slipt[3].GetString());

							font_dec->IsUnicode = (atoi(default_value->Attribute("unicode")) != 0);
						}

						InsertFontToList(name_font, font_dec);
					}

					font_detail = font_detail->NextSiblingElement();
				} while (font_detail);

			}
			Root = NULL;

		}

		TextPacker * LoadDataXML::GetTexturePackerXML()
		{
			return p_texture_packer_xml;
		}

		AtlasTexture * LoadDataXML::GetAtlastTextureByName(RKString name)
		{
			if (p_ListTextureXML.size() > 0)
			{
				if (p_ListTextureXML.find(name) != p_ListTextureXML.end())
				{
					return p_ListTextureXML.at(name);
				}
			}
			return NULL;
		}

		AtlasTexture * LoadDataXML::GetAtlasTextureByIdx(int idx)
		{
			if (p_ListTextureXML.size() > 0 && idx < p_ListTextureXML.size())
			{
				auto it = p_ListTextureXML.begin();
				while (idx > 0)
				{
					it++;
					idx--;
				}
				return it->second;
			}
			return NULL;
		}

		/*
		-----------------------------------------------------
		*/

		void  LoadDataXML::InsertFontToList(RKString name, FontDec* ddc)
		{
			p_list_font.insert(std::pair<RKString, FontDec*>(name, ddc));
		}

		FontDec*  LoadDataXML::GetFontDecByName(RKString name)
		{
			if (p_list_font.size() > 0 && p_list_font.find(name) != p_list_font.end())
			{
				return p_list_font.at(name);
			}
			return NULL;
		}

		FontDec* LoadDataXML::GetFontDecByIdx(int idx)
		{
			if (p_list_font.size() > 0)
			{
				for (auto it = p_list_font.begin(); it != p_list_font.end(); it++)
				{
					if (it->second->idx == idx)
					{
						return it->second;
					}
				}
			}
			return NULL;
		}

		/*
		-----------------------------------------------------
		*/

		Background * LoadDataXML::GetBackground(RKString str)
		{
			if (p_ListBackground.size() > 0 && p_ListBackground.find(str) != p_ListBackground.end())
				return p_ListBackground.at(str);
			return 0;
		}

		void LoadDataXML::InsertBackground(RKString str, Background * c)
		{
			p_ListBackground.insert(std::pair<RKString, Background*>(str, c));
		}

		void LoadDataXML::OnLoadBackgroundDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto BackgroundRoot = Root->FirstChildElement("BackGround");

			if (BackgroundRoot)
			{
				Vec2 designSize = ConvertToVector2(BackgroundRoot->Attribute("designSize"));

				auto BGStyle = BackgroundRoot->FirstChildElement("BGStyle");
				do
				{
					if (BGStyle)
					{

						RKString name = BGStyle->Attribute("name");
						Background * _background = new Background();
						
						auto _Layer = BGStyle->FirstChildElement("layer");
						do
						{
							if (_Layer)
							{
								LayerData * _layerData = new LayerData();
								RKString _name_layer = _Layer->Attribute("name");
								RKString _id_layer = _Layer->Attribute("id");
								_layerData->p_index_layer = atoi(_id_layer.GetString());
								_layerData->p_NameLayer = _name_layer;
								auto __Loop = _Layer->Attribute("loop");
								if (__Loop)
								{
									_layerData->p_loopAction = atoi(__Loop);
								}

								//get the config 

								auto _config_layer = _Layer->FirstChildElement("config");
								if (_config_layer)
								{
									auto _data_config = _config_layer->Attribute("delay_time");
									if (_data_config)
									{
										_layerData->p_TimeDelay = atoi(_data_config);
									}

									_data_config = _config_layer->Attribute("process_time");
									if (_data_config)
									{
										_layerData->p_processTime = atof(_data_config);
									}

									_data_config = _config_layer->Attribute("distance");
									if (_data_config)
									{
										_layerData->p_DistanceEachObject = ConvertToVector2(_data_config);
									}

									_data_config = _config_layer->Attribute("delta_distance");
									if (/*_layerData->p_DistanceEachObject != Vec2(0, 0) && */_data_config)
									{
										auto _split_x_y = RKString(_data_config).Split("_");
										_layerData->p_delta_distance_x = Vec2(atoi(_split_x_y[0].GetString()),
																			  atoi(_split_x_y[1].GetString()));
										_layerData->p_delta_distance_y = Vec2(atoi(_split_x_y[2].GetString()),
																			  atoi(_split_x_y[3].GetString()));
									}
									//get number object
									auto _child_config_layer = _config_layer->FirstChildElement("number");
									if (_child_config_layer)
									{
										_layerData->p_number_object = ConvertToVector2(_child_config_layer->Attribute("value"));
									}
									//is the random
									_child_config_layer = _config_layer->FirstChildElement("Random");
									if (_child_config_layer)
									{

										auto random_child__ = _child_config_layer->FirstChildElement("Pos");
										if (random_child__)
										{
											_layerData->p_IsObjectPosRandom = (atoi(random_child__->Attribute("random")) != 0);
											//pos
											auto __str_data = random_child__->Attribute("value");
											if (_layerData->p_IsObjectPosRandom && __str_data)
											{
												auto list__ = RKString(__str_data).Split("_");
												_layerData->p_BoundRandomPos.setRect(
													atof(list__[0].GetString()) / designSize.x,
													atof(list__[1].GetString()) / designSize.y,
													atof(list__[2].GetString())	/ designSize.x,
													atof(list__[3].GetString())	/ designSize.y
													);

											}
											__str_data = random_child__->Attribute("time");
											if (_layerData->p_IsObjectPosRandom && __str_data)
											{
												_layerData->p_TimeMoveRandom = ConvertToVector2(__str_data);
											}

											__str_data = random_child__->Attribute("delay");
											if (_layerData->p_IsObjectPosRandom && __str_data)
											{
												_layerData->p_timeDelayRandom = ConvertToVector2(__str_data);
											}

											__str_data = random_child__->Attribute("value_move_random");
											if (_layerData->p_IsObjectPosRandom && __str_data)
											{
												auto list__ = RKString(__str_data).Split("_");
												
												_layerData->p_direct_move_random_x.x = atof(list__[0].GetString())  / designSize.x;
												_layerData->p_direct_move_random_x.y = atof(list__[1].GetString())  / designSize.y;
												_layerData->p_direct_move_random_y.x = atof(list__[2].GetString())  / designSize.x;
												_layerData->p_direct_move_random_y.y = atof(list__[3].GetString())  / designSize.y;
												
											}

											__str_data = random_child__->Attribute("edge_bound");
											if (_layerData->p_IsObjectPosRandom && __str_data)
											{
												_layerData->p_idxAppearRandomOnEdgeBound = (atoi(__str_data));
											}
										}
										//scale
										random_child__ = _child_config_layer->FirstChildElement("Size");
										if (random_child__)
										{
											_layerData->p_IsScaleRandom = (atoi(random_child__->Attribute("random")) != 0);
											//random scale
											auto __str_data = random_child__->Attribute("value");
											if (__str_data && _layerData->p_IsScaleRandom)
											{
												_layerData->p_randomScale = ConvertToVector2(__str_data);
											}
										}
										//rotate
										random_child__ = _child_config_layer->FirstChildElement("Rotate");
										if (random_child__)
										{
											_layerData->p_IsRotateRandom = (atoi(random_child__->Attribute("random"))!=0);
											//random rotate
											auto __str_data = random_child__->Attribute("value");
											if (__str_data && _layerData->p_IsRotateRandom)
											{
												_layerData->p_randomRotate = ConvertToVector2(__str_data);
											}
										}

										//Opcity

										random_child__ = _child_config_layer->FirstChildElement("Opacity");
										if (random_child__)
										{
											_layerData->p_IsOpacityRandom = (atoi(random_child__->Attribute("random")) != 0);

											//random Opacity
											auto 	__str_data = random_child__->Attribute("value");
											if (__str_data && _layerData->p_IsOpacityRandom)
											{
												_layerData->p_randomOpacity = ConvertToVector2(__str_data);
											}
										}

										//color
										random_child__ = _child_config_layer->FirstChildElement("Color");
										if (random_child__)
										{
											_layerData->p_IsColorRandom = (atoi(random_child__->Attribute("random")) != 0);
											auto __str_data = random_child__->Attribute("value");
											if (__str_data && _layerData->p_IsColorRandom)
											{
												auto _color_random = RKString(__str_data).Split("_");

												_layerData->p_randomColor_r = ConvertToVector2(_color_random[0] + "_" + _color_random[1]);
												_layerData->p_randomColor_g = ConvertToVector2(_color_random[2] + "_" + _color_random[3]);
												_layerData->p_randomColor_b = ConvertToVector2(_color_random[4] + "_" + _color_random[5]);
											}
										}

										//process
										random_child__ = _child_config_layer->FirstChildElement("Process");
										if (random_child__)
										{
											_layerData->p_IsProcessRandom = (atoi(random_child__->Attribute("random")) != 0);
											auto __str_data = random_child__->Attribute("value");
											if (__str_data && _layerData->p_IsProcessRandom)
											{
												_layerData->p_percentProcessRandom = ConvertToVector2(__str_data);
												_layerData->p_timeProcessRandom = ConvertToVector2(random_child__->Attribute("time"));
											}
										}

									}

									//get position
									_child_config_layer = _config_layer->FirstChildElement("Pos");							
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "vector", "loop", "time", "delay", "delay_first" ,"type", "threadHoldRemove", "rotate_invoke" ,"reserve_move", NULL);


										Vec2 data_vector = __dec->GetDataVector2("vector");
										if (data_vector != Vec2(0,0))
										{
											data_vector.x = data_vector.x  / designSize.x;
											data_vector.y = data_vector.y  / designSize.y;

											__dec->ReplaceDataVector2("vector", data_vector);
										}

										data_vector = __dec->GetDataVector2("threadHoldRemove");
										if (data_vector != Vec2(0, 0))
										{
											data_vector.x = data_vector.x  / designSize.x;
											data_vector.y = data_vector.y  / designSize.y;

											__dec->ReplaceDataVector2("threadHoldRemove", data_vector);
										}

										data_vector = __dec->GetDataVector2("origin");
										if (data_vector != Vec2(0, 0))
										{
											data_vector.x = data_vector.x  / designSize.x;
											data_vector.y = data_vector.y  / designSize.y;

											__dec->ReplaceDataVector2("origin", data_vector);
										}

										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Pos", __dec));
									}
									//get Size
									_child_config_layer = _config_layer->FirstChildElement("Size");
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "scale", "time", "loop", "delay", "delay_first", "type", "IsFade", NULL);
										
										if (__dec->GetDatafloat("time") == 0)
										{
											__dec->ReplaceDatafloat("time", _layerData->p_processTime);
										}
										if (__dec->GetDatafloat("delay") <= 0)
										{
											__dec->ReplaceDatafloat("delay", _layerData->p_TimeDelay);
										}

										Vec2 origin_size__ = Vec2(__dec->GetDataVector2("origin").x / designSize.x, __dec->GetDataVector2("origin").y / designSize.y);
										__dec->ReplaceDataVector2("origin", origin_size__);
										float origin_scale__ = __dec->GetDatafloat("scale") / designSize.x;
										__dec->ReplaceDatafloat("scale", origin_scale__);
										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Size", __dec));
									}
									//get Rotate
									_child_config_layer = _config_layer->FirstChildElement("Rotate");
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "angle", "time", "loop", "delay", "delay_first", "type", NULL);
										if (__dec->GetDatafloat("time") == 0)
										{
											__dec->ReplaceDatafloat("time", _layerData->p_processTime);
										}
										if (__dec->GetDatafloat("delay") <= 0)
										{
											__dec->ReplaceDatafloat("delay", _layerData->p_TimeDelay);
										}

										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Rotate", __dec));
									}
									//get Opacity
									_child_config_layer = _config_layer->FirstChildElement("Opacity");
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "first", "time", "loop", "delay", "delay_first", "type", "IsFade", NULL);
										if (__dec->GetDatafloat("time") == 0)
										{
											__dec->ReplaceDatafloat("time", _layerData->p_processTime);
										}
										if (__dec->GetDatafloat("delay") <= 0)
										{
											__dec->ReplaceDatafloat("delay", _layerData->p_TimeDelay);
										}

										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Opacity", __dec));
									}
									//get Color
									_child_config_layer = _config_layer->FirstChildElement("Color");
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "target", "time", "loop", "delay", "delay_first", "type", NULL);
										if (__dec->GetDatafloat("time") == 0)
										{
											__dec->ReplaceDatafloat("time", _layerData->p_processTime);
										}
										if (__dec->GetDatafloat("delay") <= 0)
										{
											__dec->ReplaceDatafloat("delay", _layerData->p_TimeDelay);
										}

										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Color", __dec));
									}

									//get process
									_child_config_layer = _config_layer->FirstChildElement("Process");
									if (_child_config_layer)
									{
										BasicDec * __dec = GenerateBasicDec(_child_config_layer, "origin", "time", "loop", "delay", "delay_first", "type", NULL);
										if (__dec->GetDatafloat("time") == 0)
										{
											__dec->ReplaceDatafloat("time", _layerData->p_processTime);
										}
										if (__dec->GetDatafloat("delay") <= 0)
										{
											__dec->ReplaceDatafloat("delay", _layerData->p_TimeDelay);
										}

										_layerData->p_list_layer_element_dec.insert(std::pair<RKString, BasicDec *>("Process", __dec));
									}
								}

								//get the resource include
								auto _object_contain = _Layer->FirstChildElement("object_contain");
								if (_object_contain)
								{

									if (_object_contain->Attribute("IsRandom"))
									{
										_layerData->p_isObjectRandom = (atoi(_object_contain->Attribute("IsRandom")) != 0);
									}
									//load each object
									auto _object = _object_contain->FirstChildElement("object");
									do
									{
										if (_object)
										{
											ObjectDec * __object_dec = new ObjectDec();

											RKString _name_object = _object->Attribute("name");
											__object_dec->name = _name_object;
											//
											auto _child_element_object = _object->FirstChildElement("source");
											if (_child_element_object)
											{
												auto __data_str = _child_element_object->Attribute("name");
												int size_name = RKString(__data_str).Length();
												__object_dec->name_source = new char[size_name + 1];
												memcpy(__object_dec->name_source, __data_str, size_name + 1);
												
												//load animation if defined
												_child_element_object = _object->FirstChildElement("Animate");
												if (_child_element_object)
												{
													__object_dec->animation_dec = new AnimateDec();
													auto __data_str = _child_element_object->Attribute("time_delay");
													if (__data_str)
													{
														__object_dec->animation_dec->time_delay = atof(__data_str);
													}
													__data_str = _child_element_object->Attribute("loop");
													if (__data_str)
													{
														__object_dec->animation_dec->loop = atoi(__data_str);
													}
													auto __sprite_animation = _child_element_object->FirstChildElement("sprite");
													do
													{
														if (__sprite_animation)
														{
															RKString _sprite_name_source__ = __sprite_animation->Attribute("source");
															__object_dec->animation_dec->InsertSpriteFrameName(_sprite_name_source__);
															__sprite_animation = __sprite_animation->NextSiblingElement();
														}

													} while (__sprite_animation);
												}
											}
											else
											{
												_child_element_object = _object->FirstChildElement("Animature");
												if (_child_element_object)
												{
													__object_dec->animature_dec = new AnimatureDec();
													//
													auto __data_str = _child_element_object->Attribute("name");
													if (__data_str)
													{
														__object_dec->animature_dec->name = __data_str;
													}
													__data_str = _child_element_object->Attribute("SpeedScale");
													if (__data_str)
													{
														__object_dec->animature_dec->SpeedScale = atof(__data_str);
													}
													__data_str = _child_element_object->Attribute("playByIndex");
													if (__data_str)
													{
														__object_dec->animature_dec->idxPlay = atoi(__data_str);
													}
													auto _child_animature_object = _child_element_object->FirstChildElement("pathTexture");
													__object_dec->animature_dec->pathTexture = _child_animature_object->Attribute("path");

													_child_animature_object = _child_element_object->FirstChildElement("pathPlist");
													__object_dec->animature_dec->pathPlist = _child_animature_object->Attribute("path");

													_child_animature_object = _child_element_object->FirstChildElement("pathJson");
													__object_dec->animature_dec->PathJson = _child_animature_object->Attribute("path");
												}
												else
												{
													CC_ASSERT(false); //no object invalid !
												}
											}
											//get stickscreen
											_child_element_object = _object->FirstChildElement("StickScreen");
											if (_child_element_object)
											{
												__object_dec->StickScreen = atoi(_child_element_object->Attribute("value"));
											}

											//get size object
											_child_element_object = _object->FirstChildElement("Size");
											if (_child_element_object)
											{
												Vec2 value = ConvertToVector2(_child_element_object->Attribute("value"));
												__object_dec->Size_dec = new BasicDec();
												if (__object_dec->StickScreen == 1)
												{
													value.x = value.x / designSize.x;
													value.y = value.y / designSize.y;
												}
												else
												{
													value.x = value.x / designSize.x;
													value.y = value.y / designSize.x;
												}

												__object_dec->Size_dec->InsertDataVector2("origin",value);
											}
											else
											{
												Vec2 value = Vec2(50, 50);
												if (_layerData->p_list_layer_element_dec.find("Size") != _layerData->p_list_layer_element_dec.end())
												{
													value = _layerData->p_list_layer_element_dec.at("Size")->GetDataVector2("origin");
												}
												__object_dec->Size_dec = new BasicDec();
												__object_dec->Size_dec->InsertDataVector2("origin", value);
											}
											//get rotate
											_child_element_object = _object->FirstChildElement("Rotate");
											if (_child_element_object)
											{
												float value = atof(_child_element_object->Attribute("value"));
												__object_dec->Rotate_dec = new BasicDec();
												__object_dec->Rotate_dec->InsertDatafloat("origin", value);
											}
											else
											{
												float value = 0;
												if (_layerData->p_list_layer_element_dec.find("Rotate") != _layerData->p_list_layer_element_dec.end())
												{
													value = _layerData->p_list_layer_element_dec.at("Rotate")->GetDatafloat("origin");
												}
												__object_dec->Rotate_dec = new BasicDec();
												__object_dec->Rotate_dec->InsertDatafloat("origin", value);
											}
											//get opacity
											_child_element_object = _object->FirstChildElement("Opacity");
											if (_child_element_object)
											{
												RKUtils::BYTE value = atoi(_child_element_object->Attribute("value"));
												__object_dec->Opacity_dec = new BasicDec();
												__object_dec->Opacity_dec->InsertDataInt("origin", value);
											}
											else
											{
												RKUtils::BYTE value = 255;
												if (_layerData->p_list_layer_element_dec.find("Opacity") != _layerData->p_list_layer_element_dec.end())
												{
													value = _layerData->p_list_layer_element_dec.at("Opacity")->GetDataInt("origin");
												}
												__object_dec->Opacity_dec = new BasicDec();
												__object_dec->Opacity_dec->InsertDataInt("origin", value);
											}
											//get color
											_child_element_object = _object->FirstChildElement("Color");
											if (_child_element_object)
											{
												Color4B value = ConvertToColor(_child_element_object->Attribute("value"));
												__object_dec->Color_dec = new BasicDec();
												__object_dec->Color_dec->InsertDataColor("origin", value);
											}
											else
											{
												Color4B value = Color4B::WHITE;
												if (_layerData->p_list_layer_element_dec.find("Color") != _layerData->p_list_layer_element_dec.end())
												{
													value = _layerData->p_list_layer_element_dec.at("Color")->GetDataColor("origin");
												}
												__object_dec->Color_dec = new BasicDec();
												__object_dec->Color_dec->InsertDataColor("origin", value);
											}

											
											_layerData->InsertObjectBGDec(_name_object, __object_dec);

											_object = _object->NextSiblingElement();
										}
									} while (_object);
								}
								//
								_background->InsertLayerData(_id_layer + "#" + _name_layer, _layerData);
								_Layer = _Layer->NextSiblingElement();
							}

						} while (_Layer);


						InsertBackground(name, _background);

						BGStyle = BGStyle->NextSiblingElement();
					}

				} while (BGStyle);
			}
		}


		BasicDec * LoadDataXML::GenerateBasicDec(tinyxml2::XMLElement * _child_config_layer, char * name0, ...)
		{
			BasicDec * __dec = new BasicDec();
			va_list _var_list;
			char * next_arg = name0;
			//  name0 is the last argument specified; you must access 
			//  all others using the variable-argument macros.
			va_start(_var_list, name0);

			do
			{		
				if (!next_arg)
				{
					break; //run to end of list arg
				}
				auto _data_child = _child_config_layer->Attribute(next_arg);
				if (_data_child)
				{
					if (!RKString(_data_child).Contains("_"))
					{
						try
						{
							if (!RKString(_data_child).Contains("."))
							{
								int _get_data_int = atoi(_data_child);
								__dec->InsertDataInt(next_arg, _get_data_int);
							}
							else
							{
								float _get_data_float = atof(_data_child);
								__dec->InsertDatafloat(next_arg, _get_data_float);
							}
						}
						catch (exception e)
						{
							//the _data_child cannot convert to float or int so insert as char *
							__dec->InsertDataChar(next_arg, (char *)_data_child);
						}
					}
					else
					{
						//vector2 or color or vector3
						auto list__elelemt__ = RKString(_data_child).Split("_");
						if (list__elelemt__.Size() == 2)
						{
							Vec2 data_vector = ConvertToVector2(_data_child);
							__dec->InsertDataVector2(next_arg, data_vector);
						}
						else if (list__elelemt__.Size() == 4)
						{
							Color4B data_color = ConvertToColor(_data_child);
							__dec->InsertDataColor(next_arg, data_color);
						}
					}
				}
				next_arg = va_arg(_var_list, char *);

			} while (true);

			va_end(_var_list);

			return __dec;
		}
		/*
		-----------------------------------------------------
		*/
		UIMenuDec * LoadDataXML::GetUIByName(RKString str)
		{
			if (p_ListGameUI.size() > 0 && p_ListGameUI.find(str) != p_ListGameUI.end())
				return p_ListGameUI.at(str);
			return 0;
		}

		void LoadDataXML::InsertUIMenuDec(RKString str, UIMenuDec * c)
		{
			p_ListGameUI.insert(std::pair<RKString, UIMenuDec*>(str, c));
		}

		void LoadDataXML::OnLoadUIDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto rootUI = Root->FirstChildElement("menu");
			if (rootUI)
			{
				auto MenuUI = rootUI->FirstChildElement("UI");
				do
				{
#pragma region load_ui_dec
					UIMenuDec * UIdec = new UIMenuDec();
					//get first element
					RKString name_ui = MenuUI->Attribute("name");
					Vec2 design_size = GetGameDesignSize();
					if (MenuUI->Attribute("designSize"))
					{
						design_size = ConvertToVector2(MenuUI->Attribute("designSize"));
					}
					UIdec->DesignSize = design_size;
					UIdec->name = name_ui;

					//get global fade process
					auto fade_process = MenuUI->FirstChildElement("fade_process");
					Vec2 first_pos_vector(-1, -1);
					Vec2 first_scale_ratio(-1, -1);
					float first_opcity = -1;
					float first_rotate = -1;

					sound_click_dec * global_id_sound_click = NULL;
					if (fade_process)
					{
						const char * str = fade_process->Attribute("first_pos");
						if (str)
						{
							first_pos_vector = ConvertToVector2(str);
						}
						str = fade_process->Attribute("firsts_scale_ratio");
						if (str)
						{
							first_scale_ratio = ConvertToVector2(str);
						}
						str = fade_process->Attribute("first_opacity");
						if (str)
						{
							first_opcity = atof(str);
						}
						str = fade_process->Attribute("first_rotate");
						if (str)
						{
							first_rotate = atof(str);
						}
						str = fade_process->Attribute("max_idx_first");
						if (str)
						{
							UIdec->max_idx_first = atoi(str);
						}

						auto _sound_click = fade_process->FirstChildElement("sound_click");
						if (_sound_click)
						{
							global_id_sound_click = new sound_click_dec();
							auto string_sound_click = _sound_click->Attribute("value");
							if (string_sound_click)
							{
								global_id_sound_click->id_click = string_sound_click;
							}
							string_sound_click = _sound_click->Attribute("delayTime");
							if (string_sound_click)
							{
								global_id_sound_click->delayTime = atof(string_sound_click);
							}
							string_sound_click = _sound_click->Attribute("loop");
							if (string_sound_click)
							{
								global_id_sound_click->loop = atoi(string_sound_click);;
							}
						}

						// require
						auto _time_process = fade_process->FirstChildElement("time_process");
						do
						{
							if (_time_process)
							{
								float time_process = atof(_time_process->Attribute("val"));

								time_process_ui time_process__;
								time_process__.time_process = time_process;

								auto sound_process__ = _time_process->FirstChildElement("sound");
								if (sound_process__)
								{
									time_process__.ID_SOUND_FADE_IN = sound_process__->Attribute("FadeIn");
									time_process__.ID_SOUND_FADE_OUT = sound_process__->Attribute("FadeOut");
									if (sound_process__->Attribute("delayTime"))
									{
										time_process__.delayTime = atof(sound_process__->Attribute("delayTime"));
									}
									if (sound_process__->Attribute("loop"))
									{
										time_process__.loop = atoi(sound_process__->Attribute("loop"));
									}
								}
								UIdec->time_process.Insert(time_process__, atoi(_time_process->Attribute("id")));

								_time_process = _time_process->NextSiblingElement();
							}
						} while (_time_process);
					}
					//get the UI element

					auto ElementUI = MenuUI->FirstChildElement("element");
					do
					{
						int idx = atoi(ElementUI->Attribute("id"));
						RKString NameElement = ElementUI->Attribute("name");
						UIdec->InsertNameToList(NameElement);
						//
						ResUIElement * res = new ResUIElement();
						//
						res->type = ElementUI->Attribute("typeRes");
						res->source = ElementUI->Attribute("source");
						if (res->type == "Text")
						{
							//process
							if (res->source.Contains("\\n"))
							{
								RKString data__ = "";
								auto _l_str = res->source.Split("\\n");
								for (int ii = 0; ii < _l_str.Size(); ii++)
								{
									data__ += _l_str.GetAt(ii);
									if (ii != _l_str.Size() - 1)
									{
										data__ += "\n";
									}
								}
								res->source = data__;
							}

						}
						res->index = idx;
						//
						const char * visible = ElementUI->Attribute("Visible");
						if (visible)
						{
							res->visible = atoi(visible);
						}
						//
						const char * id_appear = ElementUI->Attribute("idx_appear");
						if (id_appear)
						{
							res->index_appear = atoi(id_appear);
						}
						//
						const char * fadetime = ElementUI->Attribute("time_process");
						if (fadetime)
						{
							res->TimeProcessFade = atof(fadetime);
						}
						else
						{
							if (UIdec->time_process.Size() == 1)
							{
								res->TimeProcessFade = UIdec->time_process.GetAt(0).time_process;
							}
							else
							{
								CCASSERT(res->index_appear < UIdec->time_process.Size(), "the idx_appear must be lower than size of time process");
								res->TimeProcessFade = UIdec->time_process.GetAt(res->index_appear).time_process;
							}
						}
						//
						tinyxml2::XMLElement * child_element = NULL;
						//
						if (res->type == "Panel")
						{
							const char * stick_screen = ElementUI->Attribute("StickScreen");
							if (stick_screen)
							{
								res->stick_screen = atoi(stick_screen);
							}
							else
							{
								res->stick_screen = 1;
							}
						}
						else if (res->type == "Button")
						{
							child_element = ElementUI->FirstChildElement("type_press");
							if (child_element)
							{
								res->type_press_button = atoi(child_element->Attribute("value"));
								auto child_element_btn = child_element->FirstChildElement("delta_scale");
								if (child_element_btn)
								{
									res->scale_press_down = atof(child_element_btn->Attribute("value"));
								}
								child_element_btn = child_element->FirstChildElement("hight_light");
								if (child_element_btn)
								{
									res->source_hl = child_element_btn->Attribute("value");
								}
								child_element_btn = child_element->FirstChildElement("disable");
								if (child_element_btn)
								{
									res->source_disable = child_element_btn->Attribute("value");
								}
								child_element_btn = child_element->FirstChildElement("sound_click");
								if (child_element_btn)
								{
									res->sound_click = new sound_click_dec();
									auto string_sound_click = child_element_btn->Attribute("value");
									if (string_sound_click)
									{
										res->sound_click->id_click = string_sound_click;
									}
									string_sound_click = child_element_btn->Attribute("delayTime");
									if (string_sound_click)
									{
										res->sound_click->delayTime = atof(string_sound_click);
									}
									string_sound_click = child_element_btn->Attribute("loop");
									if (string_sound_click)
									{
										res->sound_click->loop = atoi(string_sound_click);;
									}
								}
							}
						}

						if (res->sound_click==NULL)
						{
							res->sound_click = global_id_sound_click;
							if (res->sound_click == NULL)
							{
								res->sound_click = new sound_click_dec();
							}
						}
				
						//
						//require 
						child_element = ElementUI->FirstChildElement("pos");
						if (child_element)
						{
							const char * str_pos = child_element->Attribute("origin");
							if (!str_pos)
							{
								CCASSERT(false, "NOT FOUND: origin position element");
							}
							Vec2 pos = ConvertToVector2(str_pos);
							res->origin_PosRatio = Vec2(pos.x / design_size.x, pos.y / design_size.y);
							res->first_PosRatio = res->origin_PosRatio;
							//
							str_pos = child_element->Attribute("first");
							if (str_pos)
							{
								pos = ConvertToVector2(str_pos);
								res->first_PosRatio = Vec2(pos.x / design_size.x, pos.y / design_size.y);
							}
							else if (first_pos_vector != Vec2(-1, -1))
							{
								res->first_PosRatio += Vec2(first_pos_vector.x / design_size.x, first_pos_vector.y / design_size.y);
							}
							//
							str_pos = child_element->Attribute("not_fade");
							if (str_pos)
							{
								res->not_fade_pos = atoi(str_pos) == 1;
							}
							//
							if (res->first_PosRatio != res->origin_PosRatio && res->not_fade_pos == false)
							{
								res->SetActionFade(UI_STATE_FADE_ACTION::FADE_MOVE);
							}
						}
						else
						{
							CCASSERT(false, "NOT FOUND: position element");
						}
						//optional
						//
						//get scale
						child_element = ElementUI->FirstChildElement("Size");
						if (child_element)
						{
							const char * str_scale = child_element->Attribute("origin");
							if (str_scale)
							{
								Vec2 scale = ConvertToVector2(str_scale);
								//
								if (res->type == "Text")
								{
									res->origin_ScaleRatio = scale;
								}
								else
								{
									if (res->stick_screen == 1)
									{
										res->origin_ScaleRatio = Vec2(scale.x / design_size.x, scale.y / design_size.y);
									}
									else
									{
										res->origin_ScaleRatio = Vec2(scale.x / design_size.x, scale.y / design_size.x);
									}
								}
								//
								res->first_ScaleRatio = res->origin_ScaleRatio;
							}
							//
							str_scale = child_element->Attribute("first");
							if (str_scale)
							{
								Vec2 scale = ConvertToVector2(str_scale);

								if (res->type == "Text")
								{
									res->first_ScaleRatio = scale;
								}
								else
								{
									if (res->stick_screen == 1)
									{
										res->first_ScaleRatio = Vec2(scale.x / design_size.x, scale.y / design_size.y);
									}
									else
									{
										res->first_ScaleRatio = Vec2(scale.x / design_size.x, scale.x / design_size.x);
									}
								}
							}
							else if (first_scale_ratio != Vec2(-1, -1))
							{
								res->first_ScaleRatio = Vec2(res->origin_ScaleRatio.x * first_scale_ratio.x,
									res->origin_ScaleRatio.y * first_scale_ratio.y);
							}

							str_scale = child_element->Attribute("not_fade");
							if (str_scale)
							{
								res->not_fade_size = atoi(str_scale) == 1;
							}
							//
							if (res->first_ScaleRatio != res->origin_ScaleRatio && res->not_fade_size == false)
							{
								res->SetActionFade(UI_STATE_FADE_ACTION::FADE_ZOOM);
							}
						}
						//
						//get opacity
						child_element = ElementUI->FirstChildElement("opacity");
						if (child_element)
						{
							const char * str_opacity = child_element->Attribute("origin");
							if (str_opacity)
							{
								float opacity = atof(str_opacity);

								res->origin_opacity = opacity;
								res->first_opacity = res->origin_opacity;
							}
							//
							str_opacity = child_element->Attribute("first");
							if (str_opacity)
							{
								float opacity = atof(str_opacity);

								res->first_opacity = opacity;
							}
							else if (first_opcity != -1)
							{
								res->first_opacity = first_opcity;
							}
							//

							str_opacity = child_element->Attribute("not_fade");
							if (str_opacity)
							{
								res->not_fade_opacity = atoi(str_opacity) == 1;
							}

							//
							if (res->first_opacity != res->origin_opacity && res->not_fade_opacity == false)
							{
								res->SetActionFade(UI_STATE_FADE_ACTION::FADE_TRANS);
							}
						}
						//
						//get rotate
						child_element = ElementUI->FirstChildElement("rotate");
						if (child_element)
						{
							const char * str_rotate = child_element->Attribute("origin");
							if (str_rotate)
							{
								float rotate = atof(str_rotate);

								res->origin_Rotate = rotate;
								res->first_Rotate = res->origin_Rotate;
							}
							//
							str_rotate = child_element->Attribute("first");
							if (str_rotate)
							{
								float rotate = atof(str_rotate);

								res->first_Rotate = rotate;
							}
							else if (first_rotate != -1)
							{
								res->first_Rotate = first_rotate;
							}

							str_rotate = child_element->Attribute("not_fade");
							if (str_rotate)
							{
								res->not_fade_rotate = atoi(str_rotate) == 1;
							}
							//
							if (res->first_Rotate != res->origin_Rotate && res->not_fade_rotate == false)
							{
								res->SetActionFade(UI_STATE_FADE_ACTION::FADE_ROTATE);
							}
						}
						//
						//get color
						//
						child_element = ElementUI->FirstChildElement("color");
						if (child_element)
						{
							const char * str_ = child_element->Attribute("origin");
							if (str_)
							{
								auto color_tint = RKString(str_).Split("_");

								res->Tint[0] = atoi(color_tint[0].GetString());
								res->Tint[1] = atoi(color_tint[1].GetString());
								res->Tint[2] = atoi(color_tint[2].GetString());
							}
							//
							str_ = child_element->Attribute("border");
							if (str_)
							{
								auto color_tint = RKString(str_).Split("_");

								res->TintBorder[0] = atoi(color_tint[0].GetString());
								res->TintBorder[1] = atoi(color_tint[1].GetString());
								res->TintBorder[2] = atoi(color_tint[2].GetString());
								res->TintBorder[3] = atoi(color_tint[3].GetString());
							}

						}
						//
						//get font element
						//
						child_element = ElementUI->FirstChildElement("font");
						if (child_element)
						{
							const char * str_ = child_element->Attribute("idxFont");
							if (str_)
							{
								res->fontIdx = atoi(str_);
							}
							//
							str_ = child_element->Attribute("borderwd");
							if (str_)
							{
								res->BorderWidth = atof(str_);
							}
							//
							str_ = child_element->Attribute("align");
							if (str_)
							{
								res->align = atof(str_);
							}

						}
						//insert
						UIdec->InsertChildElement(NameElement, idx, res);
						//
						res = NULL;
						//
						ElementUI = ElementUI->NextSiblingElement();
						//
					} while (ElementUI);
					//insert
					InsertUIMenuDec(UIdec->name, UIdec);
					//
					/*if (global_id_sound_click)
					{
						delete global_id_sound_click;
						global_id_sound_click = NULL;
					}*/

					UIdec = NULL;
#pragma endregion load_ui_dec
					//
					MenuUI = MenuUI->NextSiblingElement();
				} while (MenuUI);
			}
		}

		/*
		-----------------------------------------------------
		*/

		UIMenuWidgetDec * LoadDataXML::GetUIWidgetByName(RKString str)
		{
			if (p_ListGameUIWidget.size() > 0 && p_ListGameUIWidget.find(str) != p_ListGameUIWidget.end())
				return p_ListGameUIWidget.at(str);
			return 0;
		}

		void LoadDataXML::InsertUIMenuWidgetDec(RKString str, UIMenuWidgetDec * c)
		{
			p_ListGameUIWidget.insert(std::pair<RKString, UIMenuWidgetDec*>(str, c));
		}

		void LoadDataXML::OnLoadUIWidgetDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
			{
				PASSERT2(false, "XML MenuWidget fail to load!");
				return;
			}

			auto rootUI = Root->FirstChildElement("list_menu_widget");
			if (rootUI)
			{
				auto UIMenuWidget = rootUI->FirstChildElement("MenuWidget");
				do
				{
					if (UIMenuWidget)
					{
#pragma region load_menu_widget_dec

						UIMenuWidgetDec * UIDec = new UIMenuWidgetDec();

						//get first element
						RKString name_ui = UIMenuWidget->Attribute("name");
						Vec2 design_size = GetGameDesignSize();
						if (UIMenuWidget->Attribute("designSize"))
						{
							design_size = ConvertToVector2(UIMenuWidget->Attribute("designSize"));
						}
						UIDec->DesignSize = design_size;
						UIDec->NameMenuWidget = name_ui;

						//get global fade process
						auto fade_process = UIMenuWidget->FirstChildElement("fade_process");
						sound_click_dec * global_id_sound_click = NULL;
						if (fade_process)
						{
							
							auto str = fade_process->Attribute("max_idx_first");
							if (str)
							{
								UIDec->max_idx_first = atoi(str);
							}

							auto _sound_click = fade_process->FirstChildElement("sound_click");
							if (_sound_click)
							{
								global_id_sound_click = new sound_click_dec();
								auto string_sound_click = _sound_click->Attribute("value");
								if (string_sound_click)
								{
									global_id_sound_click->id_click = string_sound_click;
								}
								string_sound_click = _sound_click->Attribute("delayTime");
								if (string_sound_click)
								{
									global_id_sound_click->delayTime = atof(string_sound_click);
								}
								string_sound_click = _sound_click->Attribute("loop");
								if (string_sound_click)
								{
									global_id_sound_click->loop = atoi(string_sound_click);;
								}
							}

							// require
							auto _time_process = fade_process->FirstChildElement("time_process");
							do
							{
								if (_time_process)
								{
									float time_process = atof(_time_process->Attribute("val"));

									time_process_ui time_process__;
									time_process__.time_process = time_process;

									auto sound_process__ = _time_process->FirstChildElement("sound");
									if (sound_process__)
									{
										time_process__.ID_SOUND_FADE_IN = sound_process__->Attribute("FadeIn");
										time_process__.ID_SOUND_FADE_OUT = sound_process__->Attribute("FadeOut");
										if (sound_process__->Attribute("delayTime"))
										{
											time_process__.delayTime = atof(sound_process__->Attribute("delayTime"));
										}
										if (sound_process__->Attribute("loop"))
										{
											time_process__.loop = atoi(sound_process__->Attribute("loop"));
										}
									}
									UIDec->time_process.Insert(time_process__, atoi(_time_process->Attribute("id")));

									_time_process = _time_process->NextSiblingElement();
								}
							} while (_time_process);
						}

						//get the root UI element

						auto _widget_layer_root = UIMenuWidget->FirstChildElement("Widget");
						do
						{
							if (_widget_layer_root)
							{
								auto _widget_root = OnParseWidgetEntity(_widget_layer_root, nullptr, UIDec, global_id_sound_click);
								UIDec->InsertChildElement(_widget_root->NameWidget, _widget_root->index, _widget_root);
								UIDec->InsertNameToList(_widget_root->NameWidget);
								_widget_layer_root = _widget_layer_root->NextSiblingElement();
							}

						} while (_widget_layer_root);
#pragma endregion load_menu_widget_dec

						InsertUIMenuWidgetDec(UIDec->NameMenuWidget, UIDec);

						UIMenuWidget = UIMenuWidget->NextSiblingElement();
					}

				} while (UIMenuWidget);
			}
		}

		UILayerWidgetDec * LoadDataXML::OnParseWidgetEntity(TiXmlElement * element_xml, UILayerWidgetDec* parent_widget, UIMenuWidgetDec * UIDec, sound_click_dec * global_id_sound_click)
		{
			if (!element_xml)
				return nullptr;

			UILayerWidgetDec * _widget = new UILayerWidgetDec();
			_widget->p_parent_layer = parent_widget;
			_widget->p_menu_widget = UIDec;
			_widget->index = atoi(element_xml->Attribute("id"));

			TYPE_UI_WIDGET ui_widget_type = GetUIWidget(element_xml->Attribute("typeRes"));
			PASSERT2(ui_widget_type != TYPE_UI_WIDGET::WIDGET_NONE, "ERROR: wrong widget dec!");

			_widget->typeValue = ui_widget_type;
			_widget->NameWidget = element_xml->Attribute("name");

			PINFO("INFO: widget:(%s) with type:(%s) and parent is:(%s)",
				_widget->NameWidget.GetString(),
				element_xml->Attribute("typeRes"),
				parent_widget ? parent_widget->NameWidget.GetString() : "null"
				);
			//
			if (element_xml->Attribute("visible") != nullptr)
			{
				_widget->visible = atoi(element_xml->Attribute("visible"));
			}

			if (element_xml->Attribute("idx_appear") != nullptr)
			{
				_widget->index_appear = atoi(element_xml->Attribute("idx_appear"));
			}

			if (parent_widget && _widget->index_appear <= parent_widget->index_appear)
			{
				PINFO("WARNING : The idx_appear:(%d) of child:(%s) is smaller than idx:(%d) the parent:(%s)", _widget->index_appear, _widget->NameWidget.GetString(), parent_widget->index_appear, parent_widget->NameWidget.GetString());
				PWARN1("the idx_appear of child:(%s) will be recorrect to:(%d)", _widget->NameWidget.GetString(), parent_widget->index_appear + 1);

				_widget->index_appear = parent_widget->index_appear + 1;
			}

			//time process
			if (element_xml->Attribute("time_process") != nullptr)
			{
				_widget->TimeProcessFade = atof(element_xml->Attribute("time_process"));
			}
			else
			{
					/*if (parent_widget)
					{
						PINFO("INFO: use the parent:(%s) widget time process", parent_widget->NameWidget.GetString())
						_widget->TimeProcessFade = parent_widget->TimeProcessFade;
					}
					else if (!parent_widget)*/
				{
					PINFO("INFO: use the UIMenu:(%s) widget time process", UIDec->NameMenuWidget.GetString())
						if (_widget->index_appear >= UIDec->time_process.Size())
						{
							_widget->TimeProcessFade = UIDec->time_process.GetAt(0).time_process;
						}
						else {
							_widget->TimeProcessFade = UIDec->time_process.GetAt(_widget->index_appear).time_process;
						}
					}
			}

			auto common_value = element_xml->FirstChildElement("common_value");
			if (!common_value)
			{
				PASSERT2(false, "ERROR: the common value was not found !");
				return nullptr;
			}

			auto common_obj = common_value->FirstChildElement("common");
			do
			{
				if (common_obj)
				{
					RKString name_common_value = common_obj->Attribute("name");

					RKString common_value = common_obj->Attribute("value");

					if (name_common_value == "sound_click")
					{
						_widget->sound_click = new sound_click_dec();
						_widget->sound_click->id_click = common_value;
						if (common_obj->Attribute("delay_time"))
						{
							_widget->sound_click->delayTime = atof(common_obj->Attribute("delay_time"));
						}
						else if (parent_widget && 
							(parent_widget->typeValue == WIDGET_LAYER || parent_widget->typeValue == WIDGET_LAYOUT) &&
							parent_widget->sound_click)
						{
							_widget->sound_click->delayTime = parent_widget->sound_click->delayTime;
						}
						else if (!parent_widget && global_id_sound_click)
						{
							_widget->sound_click->delayTime = global_id_sound_click->delayTime;
						}


						if (common_obj->Attribute("loop"))
						{
							_widget->sound_click->loop = atoi(common_obj->Attribute("loop"));
						}
						else if (parent_widget &&
							(parent_widget->typeValue == WIDGET_LAYER || parent_widget->typeValue == WIDGET_LAYOUT) &&
							parent_widget->sound_click)
						{
							_widget->sound_click->loop = parent_widget->sound_click->loop;
						}
						else if (!parent_widget && global_id_sound_click)
						{
							_widget->sound_click->loop = global_id_sound_click->loop;
						}
					}

					auto split_content = common_value.Split("_");

					bool isOnlyValue = true;
					try
					{
						float value_temp = stoi(split_content[0].GetString());
					}
					catch (exception e)
					{
						isOnlyValue = false;
					}

					if (split_content.Size() > 1 && isOnlyValue)
					{
						if (split_content.Size() == 4) //color
						{
							Color4B tint = ConvertToColor(common_value);
							_widget->GeneralValue->ReplaceDataColor(name_common_value, tint);
						}
						else if (split_content.Size() == 2) // vector 2
						{
							Vec2  vector = ConvertToVector2(common_value);
							_widget->GeneralValue->ReplaceDataVector2(name_common_value, vector);
						}
					}
					else
					{

						bool IsStringValue = false;

						if (name_common_value == "source" && (_widget->typeValue == WIDGET_FONT || _widget->typeValue == WIDGET_TEXT_FIELD))
						{
							IsStringValue = true;
						}

						if (!IsStringValue)
						{
							if (!common_value.Contains(".")) //string or int value
							{
								try
								{
									int value = stoi(common_value.GetString());
									_widget->GeneralValue->ReplaceDataInt(name_common_value, value);
								}
								catch (exception e)
								{
									IsStringValue = true;
								}
							}
							else
							{
								try
								{
									float value = stof(common_value.GetString());
									_widget->GeneralValue->ReplaceDatafloat(name_common_value, value);
								}
								catch (exception e)
								{
									IsStringValue = true;
								}
							}
						}

						if (IsStringValue)
						{
							RKString str_value = "";

							if (common_value.Contains("\\n"))
							{
								auto _l_str = common_value.Split("\\n");
								for (int ii = 0; ii < _l_str.Size(); ii++)
								{
									str_value += _l_str.GetAt(ii);
									if (ii != _l_str.Size() - 1)
									{
										str_value += "\n";
									}
								}
							}
							else
							{
								str_value = common_value;
							}

							_widget->GeneralValue->ReplaceDataChar(name_common_value, (char*)str_value.GetString());
						}
					}
					common_obj = common_obj->NextSiblingElement();
				}
			} while (common_obj);

			if ((_widget->typeValue == WIDGET_BUTTON || _widget->typeValue == WIDGET_CHECK_BOX || _widget->typeValue == WIDGET_SLIDER
				|| _widget->typeValue == WIDGET_LAYER || _widget->typeValue == WIDGET_LAYOUT) && _widget->sound_click == nullptr)
			{
				if (parent_widget &&
					(parent_widget->typeValue == WIDGET_LAYER || parent_widget->typeValue == WIDGET_LAYOUT) &&
					parent_widget->sound_click)
				{
					_widget->sound_click = parent_widget->sound_click;
				}
				else if (!parent_widget && global_id_sound_click)
				{
					_widget->sound_click = global_id_sound_click;
				}
				else
				{
					_widget->sound_click = new sound_click_dec();
				}
			}

			Vec2 design_size = UIDec->DesignSize;
			//parse position
			auto child_element = element_xml->FirstChildElement("pos");
			if (child_element)
			{
				const char * str_pos = child_element->Attribute("origin");
				if (!str_pos)
				{
					PASSERT2(false, "ERROR: NOT FOUND: origin position element");
				}

				Vec2 first_value, origin_value;

				Vec2 pos = ConvertToVector2(str_pos);


				origin_value = Vec2(pos.x / design_size.x, pos.y / design_size.y);
				first_value = origin_value;
				_widget->OriginValue->ReplaceDataVector2("pos_ratio", origin_value);
				_widget->FirstValue->ReplaceDataVector2("pos_ratio", first_value);
				//
				str_pos = child_element->Attribute("first");
				if (str_pos)
				{
					pos = ConvertToVector2(str_pos);
					first_value = Vec2(pos.x / design_size.x, pos.y / design_size.y);
					_widget->FirstValue->ReplaceDataVector2("pos_ratio", first_value);
				}
				//
				bool not_fade = false;
				str_pos = child_element->Attribute("not_fade");
				if (str_pos)
				{
					not_fade = atoi(str_pos) == 1;
				}
				_widget->GeneralValue->ReplaceDataInt("not_fade", (int)not_fade);
				//
				if (first_value != origin_value && not_fade == false)
				{
					_widget->SetActionFade(UI_STATE_FADE_ACTION::FADE_MOVE);
				}
			}
			else
			{
				PASSERT2(false, "ERROR: NOT FOUND: position element");
			}

			//parse scale
			child_element = element_xml->FirstChildElement("Size");
			if (child_element)
			{
				const char * str_value = child_element->Attribute("origin");
				if (!str_value)
				{
					PASSERT2(false, "ERROR: NOT FOUND: origin size element");
				}

				Vec2 first_value, origin_value;

				Vec2 size = ConvertToVector2(str_value);

				if (_widget->typeValue == WIDGET_TEXT_FIELD || _widget->typeValue == WIDGET_FONT)
				{
					origin_value = first_value = size;
				}
				else
				{
					if (_widget->GeneralValue->GetDataInt("stick_screen") == 1)
						origin_value = Vec2(size.x / design_size.x, size.y / design_size.y);
					else
						origin_value = size;
					first_value = origin_value;
				}

				_widget->OriginValue->ReplaceDataVector2("scale_ratio", origin_value);
				_widget->FirstValue->ReplaceDataVector2("scale_ratio", first_value);
				//
				str_value = child_element->Attribute("first");
				if (str_value)
				{
					size = ConvertToVector2(str_value);
					if (_widget->typeValue == WIDGET_TEXT_FIELD || _widget->typeValue == WIDGET_FONT)
					{
						first_value = size;
					}
					else
					{
						if (_widget->GeneralValue->GetDataInt("stick_screen") == 1)
							first_value = Vec2(size.x / design_size.x, size.y / design_size.y);
						else
							first_value = size;

						//first_value = Vec2(size.x / design_size.x, size.y / design_size.y);
					}
					_widget->FirstValue->ReplaceDataVector2("scale_ratio", first_value);
				}
				//
				bool not_fade = false;
				str_value = child_element->Attribute("not_fade");
				if (str_value)
				{
					not_fade = atoi(str_value) == 1;
				}
				_widget->GeneralValue->ReplaceDataInt("not_fade", (int)not_fade);
				//
				if (first_value != origin_value && not_fade == false)
				{
					_widget->SetActionFade(UI_STATE_FADE_ACTION::FADE_ZOOM);
				}
			}
			else
			{
				PASSERT2(false, "ERROR: NOT FOUND: position element");
			}

			//parse opacity
			child_element = element_xml->FirstChildElement("opacity");
			if (child_element)
			{
				const char * str_value = child_element->Attribute("origin");

				int first_value, origin_value;

				if (str_value)
				{
					first_value = origin_value = atoi(str_value);
				}
				else
				{
					first_value = origin_value = 255;
				}

				_widget->OriginValue->ReplaceDataInt("opacity", origin_value);
				_widget->FirstValue->ReplaceDataInt("opacity", first_value);
				//
				str_value = child_element->Attribute("first");
				if (str_value)
				{
					first_value = atoi(str_value);
					_widget->FirstValue->ReplaceDataInt("opacity", first_value);
				}
				//
				bool not_fade = false;
				str_value = child_element->Attribute("not_fade");
				if (str_value)
				{
					not_fade = atoi(str_value) == 1;
				}
				_widget->GeneralValue->ReplaceDataInt("not_fade", (int)not_fade);
				//
				if (first_value != origin_value && not_fade == false)
				{
					_widget->SetActionFade(UI_STATE_FADE_ACTION::FADE_TRANS);
				}
			}

			//parse rotate
			child_element = element_xml->FirstChildElement("rotate");
			if (child_element)
			{
				const char * str_value = child_element->Attribute("origin");

				float first_value, origin_value;

				if (str_value)
				{
					first_value = origin_value = atof(str_value);
				}
				else
				{
					first_value = origin_value = 0.f;
				}

				_widget->OriginValue->ReplaceDatafloat("rotate", origin_value);
				_widget->FirstValue->ReplaceDatafloat("rotate", first_value);
				//
				str_value = child_element->Attribute("first");
				if (str_value)
				{
					first_value = atof(str_value);
					_widget->FirstValue->ReplaceDatafloat("rotate", first_value);
				}
				//
				bool not_fade = false;
				str_value = child_element->Attribute("not_fade");
				if (str_value)
				{
					not_fade = atoi(str_value) == 1;
				}
				_widget->GeneralValue->ReplaceDataInt("not_fade", (int)not_fade);
				//
				if (first_value != origin_value && not_fade == false)
				{
					_widget->SetActionFade(UI_STATE_FADE_ACTION::FADE_ROTATE);
				}
			}
			
			child_element = element_xml->FirstChildElement("color");
			if (child_element)
			{
				const char * str_value = child_element->Attribute("origin");
				if (str_value)
				{
					auto color_tint = RKString(str_value).Split("_");
					_widget->GeneralValue->ReplaceDataColor("tint", Color4B(atoi(color_tint[0].GetString()), atoi(color_tint[1].GetString()), atoi(color_tint[2].GetString()), 255));
				}
			}

			//parse command

			child_element = element_xml->FirstChildElement("COMMAND");
			if (child_element)
			{
				auto command_xml = child_element->FirstChildElement("cmd");
				do
				{
					if (command_xml)
					{
						RKString name_cmd = command_xml->Attribute("name");
						RKString contain_cmd = command_xml->GetText();
	//					contain_cmd.ReplaceAll("\t", "");
	//					contain_cmd.ReplaceAll("\n", "");
						
						_widget->InsertCommand(name_cmd, contain_cmd);
						command_xml = command_xml->NextSiblingElement();
					}
				} while (command_xml);
			}


			//parse the child widget into its parent

			auto _child_widget = element_xml->FirstChildElement("Widget");
			do
			{
				if (_child_widget)
				{
					auto _child_widget_dec = OnParseWidgetEntity(_child_widget, _widget, UIDec, global_id_sound_click);
					_widget->InsertChildWidget(_child_widget_dec);

					_child_widget = _child_widget->NextSiblingElement("Widget");
				}
			} while (_child_widget);

			return _widget;
		}

		TYPE_UI_WIDGET LoadDataXML::GetUIWidget(RKString name)
		{
			if (name == "button")
				return TYPE_UI_WIDGET::WIDGET_BUTTON;
			else if (name == "panel")
				return TYPE_UI_WIDGET::WIDGET_PANEL;
			else if (name == "font")
				return TYPE_UI_WIDGET::WIDGET_FONT;
			else if (name == "check_box")
				return TYPE_UI_WIDGET::WIDGET_CHECK_BOX;
			else if (name == "loading_bar")
				return TYPE_UI_WIDGET::WIDGET_LOADING_BAR;
			else if (name == "text_field")
				return TYPE_UI_WIDGET::WIDGET_TEXT_FIELD;
			else if (name == "slider")
				return TYPE_UI_WIDGET::WIDGET_SLIDER;
			else if (name == "list_view")
				return TYPE_UI_WIDGET::WIDGET_LIST_VIEW;
			else if (name == "scroll_view")
				return TYPE_UI_WIDGET::WIDGET_SCROLL_VIEW;
			else if (name == "page_view")
				return TYPE_UI_WIDGET::WIDGET_PAGE_VIEW;
			else if (name == "layer_widget")
				return TYPE_UI_WIDGET::WIDGET_LAYER;
			else if (name == "layout")
				return TYPE_UI_WIDGET::WIDGET_LAYOUT;
			else if (name == "radio_btn")
				return TYPE_UI_WIDGET::WIDGET_RADIO_BTN;

			return TYPE_UI_WIDGET::WIDGET_NONE;
		}
		/*
		-----------------------------------------------------
		*/

		Vec2 LoadDataXML::ConvertToVector2(RKString str)
		{
			auto val = str.Split("_");
			if (val.Size() != 2)
			{
				return Vec2(0, 0);
			}
			float x = atof(val.GetAt(0).GetString());
			float y = atof(val.GetAt(1).GetString());
			return Vec2(x, y);
		}

		Color4B LoadDataXML::ConvertToColor(RKString str)
		{
			auto val = str.Split("_");
			if (val.Size() != 4)
			{
				return Color4B::WHITE;
			}
			RKUtils::BYTE r = atoi(val.GetAt(0).GetString());
			RKUtils::BYTE g = atoi(val.GetAt(1).GetString());
			RKUtils::BYTE b = atoi(val.GetAt(2).GetString());
			RKUtils::BYTE a = atoi(val.GetAt(3).GetString());
			return Color4B(r, g, b, a);
		}

		/*
		-----------------------------------------------------
		*/

		ObjectDec * LoadDataXML::GetBasicByName(RKString str)
		{
			if (p_listAbstractBasicObject.size() > 0 && p_listAbstractBasicObject.find(str) != p_listAbstractBasicObject.end())
			{
				return p_listAbstractBasicObject.at(str);
			}
			return 0;
		}

		void LoadDataXML::InsertBasicDec(RKString str, ObjectDec * c)
		{
			p_listAbstractBasicObject.insert(std::pair<RKString, ObjectDec*>(str, c));
		}

		void LoadDataXML::OnLoadListObjectDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto rootObject = Root->FirstChildElement("Object");
			if (rootObject)
			{
				Vec2 design_size = ConvertToVector2(rootObject->Attribute("designSize"));

				auto detailObject = rootObject->FirstChildElement("detailobject");
				do
				{
					if (detailObject)
					{
						ObjectDec * _object_dec = new ObjectDec();
						RKString name_object = detailObject->Attribute("name");
						_object_dec->name = name_object;
						_object_dec->idx = atoi(detailObject->Attribute("id"));
						//
						tinyxml2::XMLElement* data_element = NULL;
						//get top time process
						float time_process = 0;
						data_element = detailObject->FirstChildElement("Main");
						if (data_element)
						{
							time_process = atof(data_element->Attribute("TimeProcess"));
							//
							if (data_element->Attribute("StickScreen"))
							{
								_object_dec->StickScreen = atoi(data_element->Attribute("StickScreen"));
							}
						}
						//check source sprite
						data_element = detailObject->FirstChildElement("Source");
						if (data_element) //get sprite dec
						{
							auto name = data_element->Attribute("name");
							if (name)
							{
								int size_name_source = RKString(name).Length();
								_object_dec->name_source = new char[size_name_source + 1];
								memcpy(_object_dec->name_source, name, size_name_source + 1);
							}
						}
						else
						{
							//if not define sprite dec, get the amature
							data_element = detailObject->FirstChildElement("Animature");
							if (data_element)
							{
								_object_dec->animature_dec = new AnimatureDec();
								_object_dec->animature_dec->name = data_element->Attribute("name");
								char * data_amanture = (char *)data_element->Attribute("SpeedScale");
								if (data_amanture){
									_object_dec->animature_dec->SpeedScale = atoi(data_amanture);
								}
								data_amanture = (char *)data_element->Attribute("playByIndex");
								if (data_amanture){
									_object_dec->animature_dec->idxPlay = atoi(data_amanture);
								}
								_object_dec->animature_dec->pathTexture = data_element->FirstChildElement("pathTexture")->Attribute("path");
								_object_dec->animature_dec->pathPlist = data_element->FirstChildElement("pathPlist")->Attribute("path");
								_object_dec->animature_dec->PathJson = data_element->FirstChildElement("pathJson")->Attribute("path");

							}
							
						}
						//get animation from sprite 
						//this can do when have defined sprite
						if (_object_dec->name_source)
						{
							data_element = detailObject->FirstChildElement("Animate");
							if (data_element)
							{
								_object_dec->animation_dec = new AnimateDec();
								char * data_amantion = (char *)data_element->Attribute("time_delay");
								if (data_amantion){
									_object_dec->animation_dec->time_delay = atof(data_amantion);
								}
								data_amantion = (char *)data_element->Attribute("loop");
								if (data_amantion){
									_object_dec->animation_dec->loop = atoi(data_amantion);
								}
								auto _sprite_element = data_element->FirstChildElement("sprite");
								do
								{
									if (_sprite_element)
									{
										_object_dec->animation_dec->InsertSpriteFrameName(_sprite_element->Attribute("source"));
										_sprite_element = _sprite_element->NextSiblingElement();
									}

								} while (_sprite_element);

							}
						}
						//check the text
						data_element = detailObject->FirstChildElement("Text");
						if (data_element)
						{
							if (_object_dec->name_source)
							{
								delete _object_dec->name_source;
								_object_dec->name_source = NULL;
							}
							if (_object_dec->animation_dec)
							{
								delete _object_dec->animation_dec;
								_object_dec->animation_dec = NULL;
							}
							if (_object_dec->animature_dec)
							{
								delete _object_dec->animature_dec;
								_object_dec->animature_dec = NULL;
							}

							_object_dec->text_dec = new FontDec();

							_object_dec->text_dec->name = data_element->Attribute("name");
							_object_dec->text_dec->idx = atoi(data_element->Attribute("IdxFont"));
							_object_dec->text_dec->IsUnicode = false;
							_object_dec->text_dec->d_size = atoi(data_element->Attribute("size"));

							_object_dec->text_dec->d_size = (((float)_object_dec->text_dec->d_size / design_size.x));

						}

						//get move delay
						data_element = detailObject->FirstChildElement("MoveDelay");
						if (data_element)
						{
							_object_dec->Move_delay_dec = new BasicDec();

							const char * data_ = data_element->Attribute("pos_move_to");
							if (data_)
							{
								Vec2 pos_origin = ConvertToVector2(data_);
								pos_origin.x = pos_origin.x  / design_size.x ;
								pos_origin.y = pos_origin.y  / design_size.y;
								_object_dec->Move_delay_dec->InsertDataVector2("pos_move_to", pos_origin);
							}

							data_ = data_element->Attribute("delay");
							if (data_)
							{
								_object_dec->Move_delay_dec->InsertDatafloat("delay", atof(data_));
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Move_delay_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Move_delay_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Move_delay_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Move_delay_dec->InsertDatafloat("time", time_process);
							}
						}

						//get position aka move process
						data_element = detailObject->FirstChildElement("Pos");
						if (data_element)
						{
							_object_dec->Position_dec = new BasicDec();

							const char * data_ = data_element->Attribute("origin");
							if (data_)
							{
								Vec2 pos_origin = ConvertToVector2(data_);
								pos_origin.x = pos_origin.x / design_size.x;
								pos_origin.y = pos_origin.y / design_size.y;
								_object_dec->Position_dec->InsertDataVector2("origin", pos_origin);
							}

							data_ = data_element->Attribute("vector");
							if (data_)
							{
								Vec2 vector_move = ConvertToVector2(data_);
								vector_move.normalize();
								_object_dec->Position_dec->InsertDataVector2("vector", vector_move);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Position_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Position_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Position_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Position_dec->InsertDatafloat("time", time_process);
							}
						}


						//get size of object 
						data_element = detailObject->FirstChildElement("Size");
						if (data_element)
						{
							_object_dec->Size_dec = new BasicDec();

							const char * data_ = data_element->Attribute("origin");
							if (!data_)
							{
								CCASSERT(false, "must have origin size");
							}
							Vec2 size_origin = ConvertToVector2(data_);
							if (_object_dec->StickScreen == 1)
							{
								_object_dec->Size_dec->InsertDataVector2("origin", Vec2(size_origin.x / design_size.x, size_origin.y / design_size.y));
							}
							else
							{
								_object_dec->Size_dec->InsertDataVector2("origin", Vec2(size_origin.x / design_size.x, size_origin.y / design_size.x));
							}

							data_ = data_element->Attribute("scale");
							if (data_)
							{
								float delta_scale = atof(data_);
								_object_dec->Size_dec->InsertDatafloat("scale", delta_scale / design_size.x);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Size_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Size_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Size_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Size_dec->InsertDatafloat("time", time_process);
							}
						}
						else
						{
							CCASSERT(false, "Must have the origin size of object !");
						}

						//get opacity
						data_element = detailObject->FirstChildElement("Opacity");
						if (data_element)
						{
							_object_dec->Opacity_dec = new BasicDec();
							const char * data_ = data_element->Attribute("origin");
							if (data_)
							{
								RKUtils::BYTE opaciy_origin = atoi(data_);
								_object_dec->Opacity_dec->InsertDataInt("origin", opaciy_origin);
							}
							else
							{
								_object_dec->Opacity_dec->InsertDataInt("origin", 255);
							}

							data_ = data_element->Attribute("first");
							if (data_)
							{
								RKUtils::BYTE first_opacity = atof(data_);
								_object_dec->Opacity_dec->InsertDataInt("first", first_opacity);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Opacity_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Opacity_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Opacity_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Opacity_dec->InsertDatafloat("time", time_process);
							}
						}
						else
						{
							_object_dec->Opacity_dec = new BasicDec();
							_object_dec->Opacity_dec->InsertDataInt("origin", 255);
						}

						//get rotate
						data_element = detailObject->FirstChildElement("Rotate");
						if (data_element)
						{
							_object_dec->Rotate_dec = new BasicDec();
							const char * data_ = data_element->Attribute("origin");
							if (data_)
							{
								float angle_origin = atoi(data_);
								_object_dec->Rotate_dec->InsertDatafloat("origin", angle_origin);
							}
							else
							{
								_object_dec->Rotate_dec->InsertDatafloat("origin", 0);
							}

							data_ = data_element->Attribute("angle");
							if (data_)
							{
								float angle_target = atof(data_);
								_object_dec->Rotate_dec->InsertDatafloat("angle", angle_target);
							}
							else
							{
								_object_dec->Rotate_dec->InsertDatafloat("angle", 360.f);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Rotate_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Rotate_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Rotate_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Rotate_dec->InsertDatafloat("time", time_process);
							}
						}

						//get rotate around object
						data_element = detailObject->FirstChildElement("RotateObj");
						if (data_element)
						{
							_object_dec->Rotate_around_dec = new BasicDec();

							const char * data_ = data_element->Attribute("distance");
							if (data_)
							{
								float distance = atoi(data_);
								_object_dec->Rotate_around_dec->InsertDatafloat("distance", distance);
							}
							else
							{
								_object_dec->Rotate_around_dec->InsertDatafloat("distance", 1);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Rotate_around_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Rotate_around_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Rotate_around_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Rotate_around_dec->InsertDatafloat("time", time_process);
							}
						}

						//get color
						data_element = detailObject->FirstChildElement("Color");
						if (data_element)
						{
							_object_dec->Color_dec = new BasicDec();
							const char * data_ = data_element->Attribute("origin");
							if (data_)
							{
								Color4B origin_color = ConvertToColor(data_);
								_object_dec->Color_dec->InsertDataColor("origin", origin_color);
							}
							else
							{
								_object_dec->Color_dec->InsertDataColor("origin", Color4B::WHITE);
							}

							data_ = data_element->Attribute("target");
							if (data_)
							{
								Color4B target_color = ConvertToColor(data_);
								_object_dec->Color_dec->InsertDataColor("target", target_color);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Color_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Color_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Color_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Color_dec->InsertDatafloat("time", time_process);
							}


						}

						data_element = detailObject->FirstChildElement("Process");
						if (data_element)
						{
							_object_dec->Process_dec = new BasicDec();
							const char * data_ = data_element->Attribute("origin");
							if (data_)
							{
								float percent_origin = atof(data_);
								_object_dec->Process_dec->InsertDatafloat("origin", percent_origin);
							}
							else
							{
								_object_dec->Process_dec->InsertDatafloat("origin",100.f);
							}

							data_ = data_element->Attribute("loop");
							if (data_)
							{
								int loop = atoi(data_);
								_object_dec->Process_dec->InsertDataInt("loop", loop);
							}
							else
							{
								_object_dec->Process_dec->InsertDataInt("loop", 0);
							}

							data_ = data_element->Attribute("time");
							if (data_)
							{
								float time = atof(data_);
								_object_dec->Process_dec->InsertDatafloat("time", time);
							}
							else
							{
								_object_dec->Process_dec->InsertDatafloat("time", time_process);
							}


						}

						InsertBasicDec(name_object, _object_dec);
						detailObject = detailObject->NextSiblingElement();
					}
				} while (detailObject);

			}
		}

		/*
		-----------------------------------------------------
		*/

		Wide_ObjectDec *  LoadDataXML::GetWideObjectByName(RKString str)
		{
			if (p_listWideObject.size() > 0 && p_listWideObject.find(str) != p_listWideObject.end())
			{
				return p_listWideObject.at(str);
			}
			return 0;
		}

		void LoadDataXML::InsertWideObjectDec(RKString str, Wide_ObjectDec * c)
		{
			p_listWideObject.insert(std::pair<RKString, Wide_ObjectDec*>(str, c));
		}

		Wide_ObjectDec * LoadDataXML::OnParseObjectWide(TiXmlElement * element_xml, Wide_ObjectDec* parent_object, Vec2 design_size)
		{
			if (element_xml)
			{
				Wide_ObjectDec * _object = new Wide_ObjectDec();

				_object->name_object = element_xml->Attribute("name");
				_object->parent_object = parent_object;
				_object->design_size = design_size;
				auto attribute = element_xml->FirstChildElement("Main");
				if (attribute)
				{
					_object->type_object = atoi(attribute->Attribute("type_object"));
					_object->CommonValue->ReplaceDataInt("stick_screen", atoi(attribute->Attribute("stick_screen")));

					_object->CommonValue->ReplaceDataInt("visible", atoi(attribute->Attribute("visible")));
				}
				else
				{
					PASSERT2(attribute != nullptr, "ERROR : main attribute not found !");
					return nullptr;
				}

				if (_object->type_object == 0 || _object->type_object == 4) //sprite or process timer
				{
					attribute = element_xml->FirstChildElement("Source");
					if (attribute)
					{
						_object->name_source = attribute->Attribute("name");
					}
					else
					{
						PASSERT2(attribute != nullptr, "ERROR : source attribute not found !");
						return nullptr;
					}
				}
				else if (_object->type_object == 1)
				{
					attribute = element_xml->FirstChildElement("Animature");
					if (attribute)
					{
						_object->animature_dec = new AnimatureDec();
						_object->animature_dec->name = attribute->Attribute("name");
						char * data_amanture = (char *)attribute->Attribute("speed_scale");
						if (data_amanture){
							_object->animature_dec->SpeedScale = atoi(data_amanture);
						}

						data_amanture = (char *)attribute->Attribute("playByIndex");
						if (data_amanture){
							_object->animature_dec->idxPlay = atoi(data_amanture);
						}

						_object->animature_dec->pathTexture = attribute->FirstChildElement("pathTexture")->Attribute("path");
						_object->animature_dec->pathPlist = attribute->FirstChildElement("pathPlist")->Attribute("path");
						_object->animature_dec->PathJson = attribute->FirstChildElement("pathJson")->Attribute("path");
					}
					else
					{
						PASSERT2(attribute != nullptr, "ERROR : Animature attribute not found !");
						return nullptr;
					}
				}
				else if (_object->type_object == 2)
				{
					attribute = element_xml->FirstChildElement("Animate");
					if (attribute)
					{
						_object->animation_dec = new AnimateDec();
						_object->animation_dec->time_delay = atof(attribute->Attribute("time_delay"));
						_object->animation_dec->time_delay = atoi(attribute->Attribute("loop"));
						_object->animation_dec->source_sprite = attribute->Attribute("source");
						_object->animation_dec->tag_action = atoi(attribute->Attribute("tag"));
						auto _sprite_element = attribute->FirstChildElement("sprite");
						do
						{
							if (_sprite_element)
							{
								_object->animation_dec->InsertSpriteFrameName(_sprite_element->Attribute("source"));
								_sprite_element = _sprite_element->NextSiblingElement();
							}

						} while (_sprite_element);
					}
					else
					{
						PASSERT2(attribute != nullptr, "ERROR : animation attribute not found !");
						return nullptr;
					}
				}
				else if (_object->type_object == 3)
				{
					attribute = element_xml->FirstChildElement("Text");
					if (attribute)
					{
						_object->text_dec = new FontDec();

						_object->text_dec->name = attribute->Attribute("name");
						_object->text_dec->idx = atoi(attribute->Attribute("IdxFont"));
						_object->text_dec->IsUnicode = false;
						_object->text_dec->d_size = atoi(attribute->Attribute("size"));

						_object->text_dec->d_size = (((float)_object->text_dec->d_size / design_size.x));

						if (attribute->Attribute("border_w"))
						{
							_object->text_dec->d_width_border = atof(attribute->Attribute("border_w"));

							if (attribute->Attribute("border_color"))
							{
								Color4B tint_border = ConvertToColor(attribute->Attribute("border_color"));
								_object->text_dec->d_color_border[0] = tint_border.r;
								_object->text_dec->d_color_border[1] = tint_border.g;
								_object->text_dec->d_color_border[2] = tint_border.b;
								_object->text_dec->d_color_border[3] = tint_border.a;
							}
						}
						if (attribute->Attribute("color"))
						{
							Color4B tint_ = ConvertToColor(attribute->Attribute("color"));
							_object->text_dec->d_color[0] = tint_.r;
							_object->text_dec->d_color[1] = tint_.g;
							_object->text_dec->d_color[2] = tint_.b;
							_object->text_dec->d_color[3] = tint_.a;
						}

					}
					else
					{
						PASSERT2(attribute != nullptr, "ERROR : text attribute not found !");
						return nullptr;
					}
				}

				//parse origin value

				attribute = element_xml->FirstChildElement("Pos");
				if (attribute)
				{
					Vec2 value = ConvertToVector2(attribute->Attribute("origin"));
					value = Vec2(value.x / design_size.x, value.y / design_size.y);
					_object->OriginValue->ReplaceDataVector2("pos_ratio", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					Vec2 vecfirst_value = ConvertToVector2(first_value);
					vecfirst_value = Vec2(vecfirst_value.x / design_size.x, vecfirst_value.y / design_size.y);
					_object->FirstValue->ReplaceDataVector2("pos_ratio", vecfirst_value);
				}

				attribute = element_xml->FirstChildElement("Size");
				if (attribute)
				{
					Vec2 value = ConvertToVector2(attribute->Attribute("origin"));
					value = Vec2(value.x / design_size.x, value.y / design_size.x);
					_object->OriginValue->ReplaceDataVector2("scale_ratio", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					Vec2 vecfirst_value = ConvertToVector2(first_value);
					vecfirst_value = Vec2(vecfirst_value.x / design_size.x, vecfirst_value.y / design_size.x);
					_object->FirstValue->ReplaceDataVector2("scale_ratio", vecfirst_value);
				}

				attribute = element_xml->FirstChildElement("Opacity");
				if (attribute)
				{
					int value = atoi(attribute->Attribute("origin"));
					_object->OriginValue->ReplaceDataInt("opacity", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					_object->FirstValue->ReplaceDataInt("opacity", atoi(first_value));
				}

				attribute = element_xml->FirstChildElement("Rotate");
				if (attribute)
				{
					float value = atof(attribute->Attribute("origin"));
					_object->OriginValue->ReplaceDatafloat("rotate", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					_object->FirstValue->ReplaceDatafloat("rotate", atof(first_value));
				}

				attribute = element_xml->FirstChildElement("Process");
				if (attribute)
				{
					int value = atoi(attribute->Attribute("origin"));
					_object->OriginValue->ReplaceDataInt("Process", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					_object->FirstValue->ReplaceDataInt("Process", atoi(first_value));
				}

				attribute = element_xml->FirstChildElement("Color");
				if (attribute)
				{
					Color4B value = ConvertToColor(attribute->Attribute("origin"));
					_object->OriginValue->ReplaceDataColor("tint", value);

					const char * first_value = attribute->Attribute("origin");
					if (attribute->Attribute("first"))
					{
						first_value = attribute->Attribute("first");
					}
					_object->FirstValue->ReplaceDataColor("tint", ConvertToColor(first_value));
				}

				//parse command

				attribute = element_xml->FirstChildElement("COMMAND");
				if (attribute)
				{
					auto command_xml = attribute->FirstChildElement("cmd");
					do
					{
						if (command_xml)
						{
							RKString name_cmd = command_xml->Attribute("name");
							RKString contain_cmd = command_xml->GetText();

							_object->InsertCommand(name_cmd, contain_cmd);
							command_xml = command_xml->NextSiblingElement();
						}
					} while (command_xml);
				}

				//parse object child
				attribute = element_xml->FirstChildElement("detailobject");
				do
				{
					if (attribute)
					{
						auto _child_dec = OnParseObjectWide(attribute, _object, design_size);
						_object->InsertWideObjectChild(_child_dec);

						attribute = attribute->NextSiblingElement("detailobject");
					}
				} while (attribute);

				return _object;
			}


			return nullptr;
		}

		void  LoadDataXML::OnLoadListWideObjectDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			RKString name_object = "Object";
			if (GetGameSize().width / GetGameSize().height > 0.68)
			{
				name_object = "Object_ipad";
			}
			auto rootObject = Root->FirstChildElement(name_object.GetString());
			if (rootObject)
			{
				Vec2 design_size = ConvertToVector2(rootObject->Attribute("designSize"));

				auto detailObject = rootObject->FirstChildElement("detailobject");
				do
				{
					if (detailObject)
					{			
						auto w_object = OnParseObjectWide(detailObject, nullptr, ConvertToVector2(rootObject->Attribute("designSize")));
						
						p_listWideObject.insert(std::pair<RKString, Wide_ObjectDec*>(w_object->name_object, w_object));

						detailObject = detailObject->NextSiblingElement();
					}
				} while (detailObject);

			}
		}

		/*
		-----------------------------------------------------
		*/

		MaterialDec * LoadDataXML::GetMaterialByName(RKString str)
		{
			if (p_listMaterial.size() > 0 && p_listMaterial.find(str) != p_listMaterial.end())
			{
				return p_listMaterial.at(str);
			}
			return 0;
		}

		void LoadDataXML::InsertMaterialDec(RKString str, MaterialDec * c)
		{
			p_listMaterial.insert(std::pair<RKString,MaterialDec *>(str,c));
		}

		ShaderDec * LoadDataXML::GetShaderByName(RKString str)
		{
			if (p_listShaders.size() > 0 && p_listShaders.find(str) != p_listShaders.end())
			{
				return p_listShaders.at(str);
			}
			return 0;
		}

		void LoadDataXML::InsertShaderDec(RKString str, ShaderDec * c)
		{
			p_listShaders.insert(std::pair<RKString, ShaderDec *>(str, c));
		}

		void LoadDataXML::OnLoadMaterialDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto rootMaterial = Root->FirstChildElement("Material");
			if (rootMaterial)
			{
				auto material_dec = rootMaterial->FirstChildElement("material_dec");
				do
				{
					if (material_dec)
					{

						MaterialDec * m_dec = new MaterialDec();
						m_dec->Path = material_dec->Attribute("Path");
						m_dec->namespacePath = material_dec->Attribute("namespacePath");
						RKString name = material_dec->Attribute("name");

						auto technique_dec = material_dec->FirstChildElement("technique");
						do
						{
							if (technique_dec)
							{
								techniqueMaterialDec * tech_dec = new techniqueMaterialDec(technique_dec->Attribute("name"));
								auto pass_dec = technique_dec->FirstChildElement("pass");
								do
								{
									if (pass_dec)
									{
										tech_dec->InsertPass(pass_dec->Attribute("name"));
										pass_dec = pass_dec->NextSiblingElement();
									}
								} while (pass_dec);

								m_dec->InsertTechiqueByname(tech_dec->GetName(), tech_dec);
								technique_dec = technique_dec->NextSiblingElement();
							}
						} while (technique_dec);

						InsertMaterialDec(name, m_dec);
						material_dec = material_dec->NextSiblingElement();
					}
				} while (material_dec);
			}
			rootMaterial = Root->FirstChildElement("Shader");
			if (rootMaterial)
			{
				auto shader_dec = rootMaterial->FirstChildElement("shader");
				do
				{
					if (shader_dec)
					{
						RKString name = shader_dec->Attribute("name");
						ShaderDec * shader_ = new ShaderDec();
						shader_->p_name = name;
						
						auto name_shader_ = shader_dec->FirstChildElement("HLSL");
						if (name_shader_)
						{
							shader_->p_name_pixel_dx = name_shader_->Attribute("pixel_shader");
							shader_->p_name_vert_dx = name_shader_->Attribute("vert_shader");
						}
						name_shader_ = shader_dec->FirstChildElement("GLSL");
						if (name_shader_)
						{
							shader_->p_name_pixel_gl = name_shader_->Attribute("pixel_shader");
							shader_->p_name_vert_gl = name_shader_->Attribute("vert_shader");
						}
						InsertShaderDec(name, shader_);

						shader_dec = shader_dec->NextSiblingElement();
					}
				} while (shader_dec);
			}

			rootMaterial = NULL;
		}

		/*
		-----------------------------------------------------
		*/

		SoundDec * LoadDataXML::GetSoundDecByName(RKString str)
		{
			if (p_listSoundDec.size() > 0 && p_listSoundDec.find(str) != p_listSoundDec.end())
			{
				return p_listSoundDec.at(str);
			}
			return 0;
		}
		
		void LoadDataXML::InsertSoundDec(RKString str, SoundDec * c)
		{
			p_listSoundDec.insert(std::pair<RKString, SoundDec *>(str, c));
		}

		void LoadDataXML::OnLoadListSoundDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto rootSound = Root->FirstChildElement("Sound");
			if (rootSound)
			{
				auto sound_detail = rootSound->FirstChildElement("sound_play");
				do
				{
					if (sound_detail)
					{
						SoundDec * __sound_dec = new SoundDec();

						RKString name_id = sound_detail->Attribute("name");
						__sound_dec->name_id = name_id;
						auto detail_element = sound_detail->FirstChildElement("path");
						if (detail_element)
						{
							__sound_dec->path = detail_element->Attribute("value");
						}
						detail_element = sound_detail->FirstChildElement("delay_first");
						if (detail_element)
						{
							__sound_dec->delay_first = atof(detail_element->Attribute("value"));
						}
						detail_element = sound_detail->FirstChildElement("loop");
						if (detail_element)
						{
							__sound_dec->loop = atoi(detail_element->Attribute("value"));
						}
						detail_element = sound_detail->FirstChildElement("type");
						if (detail_element)
						{
							__sound_dec->type_sound = detail_element->Attribute("value");
						}

						InsertSoundDec(name_id, __sound_dec);

						sound_detail = sound_detail->NextSiblingElement();
					}


				} while (sound_detail);

			}


		}

		/*
		-----------------------------------------------------
		*/

		EffectDec * LoadDataXML::GetEffectDecByName(RKString str)
		{
			if (p_listEffectDec.size() > 0 && p_listEffectDec.find(str) != p_listEffectDec.end())
			{
				return p_listEffectDec.at(str);
			}
			return 0;
		}
		void LoadDataXML::InsertEffectDec(RKString str, EffectDec * c)
		{
			p_listEffectDec.insert(std::pair<RKString, EffectDec *>(str, c));
		}
		void LoadDataXML::OnLoadListEffectDecXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto RpootEffect = Root->FirstChildElement("Effect");
			if (RpootEffect)
			{
				auto effect_detail = RpootEffect->FirstChildElement("eff");
				do
				{
					if (effect_detail)
					{
						EffectDec * __effect_dec = new EffectDec();

						RKString name_effect = effect_detail->Attribute("name");
						__effect_dec->name_effect = name_effect;
						auto __element_child = effect_detail->FirstChildElement("source_plist");
						if (__element_child)
						{
							__effect_dec->name_source_plist = __element_child->Attribute("value");
						}
						__element_child = effect_detail->FirstChildElement("source_texture");
						if (__element_child)
						{
							__effect_dec->name_source_tex = __element_child->Attribute("value");
						}

						InsertEffectDec(name_effect, __effect_dec);

						effect_detail = effect_detail->NextSiblingElement();
					}


				} while (effect_detail);

			}
		}

		/*
		-----------------------------------------------------
		*/

		Scene3D * LoadDataXML::GetScene3DXMLByName(RKString str)
		{
			if (p_listScene3D.size() > 0 && p_listScene3D.find(str) != p_listScene3D.end())
			{
				return p_listScene3D.at(str);
			}
			return 0;
		}
		void LoadDataXML::InsertScene3DXML(RKString str, Scene3D * c)
		{
			p_listScene3D.insert(std::pair<RKString, Scene3D *>(str, c));
		}
		void LoadDataXML::OnLoadListScene3DXML(TiXmlDocument * objectXMl)
		{
			auto Root = objectXMl->FirstChildElement("GAME");
			if (!Root)
				return;

			auto RootScene = Root->FirstChildElement("ListScene");
			if (RootScene)
			{
				auto scene_detail = RootScene->FirstChildElement("Scene");
				do
				{
					if (scene_detail)
					{
						Scene3D * _scene = new Scene3D();
						RKString name_scene = scene_detail->Attribute("name");

						auto _detail_object = scene_detail->FirstChildElement("skybox");
						if (_detail_object)
						{
							auto detail_tex_skybox = _detail_object->FirstChildElement("tex");
							do
							{
								if (detail_tex_skybox)
								{
									RKString type = detail_tex_skybox->Attribute("type");
									RKString name_path = detail_tex_skybox->Attribute("name");

									_scene->skybox_tex->InsertDataChar(type, name_path.GetString());

									detail_tex_skybox = detail_tex_skybox->NextSiblingElement();
								}
							} while (detail_tex_skybox);
						}
						else
						{
							PASSERT2(_detail_object != nullptr, "skybox must be defined!");
						}

						_detail_object = scene_detail->FirstChildElement("terrain");
						if (_detail_object)
						{
							_scene->terrain_tex->InsertDataVector2("chunk_size", ConvertToVector2(_detail_object->Attribute("chunk_size")));
							_scene->terrain_tex->InsertDatafloat("map_height", atof(_detail_object->Attribute("map_height")));
							_scene->terrain_tex->InsertDatafloat("map_scale", atof(_detail_object->Attribute("map_scale")));

							auto detail_tex_skybox = _detail_object->FirstChildElement("tex");
							do
							{
								if (detail_tex_skybox)
								{
									RKString type = detail_tex_skybox->Attribute("type");
									RKString name_path = detail_tex_skybox->Attribute("name");

									_scene->terrain_tex->InsertDataChar(type, name_path.GetString());

									detail_tex_skybox = detail_tex_skybox->NextSiblingElement();
								}
							} while (detail_tex_skybox);
						}
						else
						{
							PASSERT2(_detail_object != nullptr, "terrain must be defined!");
						}

						InsertScene3DXML(name_scene, _scene);

						scene_detail = scene_detail->NextSiblingElement();
					}
				} while (scene_detail);

			}
		}
	}
}
