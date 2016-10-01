#include "DisplayView.h"
#include "ObjectTexture.h"

namespace Utility
{
	namespace Texture
	{
		ObjectTexture * ObjectTexture::p_Instance = NULL;
		ObjectTexture::ObjectTexture()
		{
			p_levelMipMap = 0;
		}

		///////////////////////////////////////////////////////////////////////////////
		ObjectTexture::~ObjectTexture()
		{
			UnloadData();
		}
		
		///////////////////////////////////////////////////////////////////////////////
		Texture2D * ObjectTexture::GetTextureByName(RKString nameTexture)
		{
			if (p_listTexture.size() > 0)
			{
				if (p_listTexture.find(nameTexture) != p_listTexture.end())
				{
					return p_listTexture.at(nameTexture);
				}
			}
			return 0;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		Sprite * ObjectTexture::GetSpriteByName(RKString nameSprite)
		{
#ifdef USE_SPRITE_FRAME
			Sprite * p = nullptr;
			p = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(nameSprite.GetString()));
			return p;
#else
			if (p_listSprite.size() > 0)
			{
				if (p_listSprite.find(nameSprite) != p_listSprite.end())
				{
					return p_listSprite.at(nameSprite);
				}
			}

			return 0;
#endif
			
		}
		
		///////////////////////////////////////////////////////////////////////////////
		Label * ObjectTexture::GetFontByName(RKString nameFont)
		{
			if (p_listFont.size() > 0)
			{
				if (p_listFont.find(nameFont) != p_listFont.end())
				{
					return p_listFont.at(nameFont)->label;
				}
			}
			return 0;
		}
		Label *	ObjectTexture::GetFontByIdx(int idx)
		{
			if (p_listFont.size() > 0)
			{
				for (auto iterator = p_listFont.begin(); iterator != p_listFont.end(); ++iterator)
				{
					if (iterator->second->idx == idx)
					{
						return iterator->second->label;
					}
				}
			}
			return 0;
		}
		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::InsertTextureByName(RKString name, Texture2D * tex)
		{
			p_listTexture.insert(std::pair<RKString, Texture2D *>(name, tex));
		}
		
		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::InsertSpriteByName(RKString name, Sprite * tex)
		{
			p_listSprite.insert(std::pair<RKString, Sprite *>(name, tex));
		}
		
		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::InsertFontByName(RKString name, LabelDetail * tex)
		{
			p_listFont.insert(std::pair<RKString, LabelDetail *>(name, tex));
		}

		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::UnloadData()
		{
			p_listTexture.clear();
			p_listSprite.clear();
			p_listFont.clear();
		}

		///////////////////////////////////////////////////////////////////////////////
		Texture2D * ObjectTexture::CreateTextureFromNamePath(RKString namepath, bool loadAA , int levelMipMap)
		{
			//create null image
			Image * p_AtlatSpriteObjectTemp = NULL;

			//create texture null
			Texture2D * p_tex = new Texture2D();

			//init image from namepath
			p_AtlatSpriteObjectTemp = new Image();
			p_AtlatSpriteObjectTemp->initWithImageFile(namepath.GetString());
			//
			if (p_AtlatSpriteObjectTemp->getNumberOfMipmaps() > 1)
			{
				int w_mip = p_AtlatSpriteObjectTemp->getWidth() >> levelMipMap;
				int h_mip = p_AtlatSpriteObjectTemp->getHeight() >> levelMipMap;
				//
				p_tex->initWithData(
					p_AtlatSpriteObjectTemp->getMipmaps()[levelMipMap].address, 
					p_AtlatSpriteObjectTemp->getMipmaps()[levelMipMap].len,
					p_AtlatSpriteObjectTemp->getRenderFormat(), 
					w_mip,
					h_mip,
                                    cocos2d::Size(w_mip, h_mip)
					);
			}
			else
			{
				p_tex->initWithImage(p_AtlatSpriteObjectTemp);				
			}

#ifdef USE_ANTIALIAS
			if (loadAA)
			{
				p_tex->generateMipmap();
				Texture2D::TexParams texParams = { GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE };
				p_tex->setTexParameters(&texParams);
			}
#endif

			CC_SAFE_RELEASE_NULL(p_AtlatSpriteObjectTemp);

			return p_tex;
		}
		
		///////////////////////////////////////////////////////////////////////////////
		Sprite * ObjectTexture::CreateSpriteFromTexture(Texture2D * tex, xml::AtlasChild* nameSprite, int levelMipMap)
		{
			cocos2d::Rect bound;
			bound.setRect(
				float((int)nameSprite->Position.x >> levelMipMap),
				float((int)nameSprite->Position.y >> levelMipMap),
				float((int)nameSprite->Size.x >> levelMipMap),
				float((int)nameSprite->Size.y >> levelMipMap)
				);

			auto sprite = Sprite::createWithTexture(tex, bound);

			return sprite;

		}

		void ObjectTexture::LoadTexturePacker(RKString name, bool loadAA)
		{
			bool IsUseArk = false;
#ifdef USE_ARK_ARCHIVE
			IsUseArk = true;
			g_IsUseArk = IsUseArk;
#endif
			if (XMLMgr->GetHarwardProfile() == xml::HARDWARE_HIGH || XMLMgr->GetHarwardProfile() == xml::HARDWARE_MED)
			{
				p_levelMipMap = 0;
			}
			else
			{
				p_levelMipMap = 1;
			}
			RKString texture_path = "";
			RKString plist_path = "";

			int number_texture = XMLMgr->GetTexturePackerXML()->GetNumberTextureByName(name);
			while (number_texture > 0)
			{
				auto tex_pack = XMLMgr->GetTexturePackerXML()->GetTexPackByName(name , number_texture - 1);

				if (tex_pack.IsValid())
				{
					texture_path = File::ResolveNamePath(tex_pack.texture_path);
					plist_path = File::ResolveNamePath(tex_pack.texture_plist);
				}
				if (texture_path != "")
				{
					cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile(plist_path.GetString(), texture_path.GetString());
				}

				number_texture--;
			}
		

		}

		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::Load(bool loadAA)
		{
			bool IsUseArk = false;
#ifdef USE_ARK_ARCHIVE
			IsUseArk = true;
			g_IsUseArk = IsUseArk;
#endif
			if (XMLMgr->GetHarwardProfile() == xml::HARDWARE_HIGH || XMLMgr->GetHarwardProfile() == xml::HARDWARE_MED)
			{
				p_levelMipMap = 0;
			}
			else
			{
				p_levelMipMap = 1;
			}

			//loop through all the texture data store in xml
			int number_image = XMLMgr->GetNumberTexture();
			while (number_image > 0)
			{
				number_image--;
				//process each image
				auto textureXml = XMLMgr->GetAtlasTextureByIdx(number_image);

				RKString namepath = textureXml->NamePath;
                
				if (p_levelMipMap > 0 && namepath.Contains(".png"))
				{
					namepath = File::GenerateMipName(namepath);
				}
				RKString ImagePath = File::ResolveNamePath(namepath);

				// create texture and insert to game
				auto tex = CreateTextureFromNamePath(ImagePath, loadAA, p_levelMipMap);
				InsertTextureByName(textureXml->NameImage, tex);

				//create list sprite from tex and insert to game
				for (auto it = textureXml->dDataSize.begin(); it != textureXml->dDataSize.end(); it++)
				{
					auto sprite = CreateSpriteFromTexture(tex, &it->second, p_levelMipMap);
#ifdef USE_SPRITE_FRAME
					//store into the sprite frame
					SpriteFrame * sp = SpriteFrame::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
					SpriteFrameCache::getInstance()->addSpriteFrame(sp, it->first.GetString());
#else
                    //store into the sprite frame
                    SpriteFrame * sp = SpriteFrame::createWithTexture(sprite->getTexture(), sprite->getTextureRect());
                    SpriteFrameCache::getInstance()->addSpriteFrame(sp, it->first.GetString());
                    
					sprite->retain();
					InsertSpriteByName(it->first, sprite);
#endif
				}
				//
				tex = NULL;
			}

			//
			//
			//load font
			//
			p_listFont.clear();
			size_t numberfont = XMLMgr->GetNumberFont();
			for (size_t i = 0; i < numberfont; i++)
			{
				auto font_dec = XMLMgr->GetFontDecByIdx((int)i);
				Label* string = NULL;
				string = Label::createWithTTF(						
						"",
						File::ResolveNamePath(font_dec->path).GetString(),
						font_dec->d_size
						);
				string->setColor(Color3B(font_dec->d_color[0], font_dec->d_color[1], font_dec->d_color[2]));
				string->setOpacity(font_dec->d_color[3]);
				if (font_dec->d_width_border > 0)
				{
					string->enableOutline(Color4B(font_dec->d_color_border[0], font_dec->d_color_border[1], font_dec->d_color_border[2], font_dec->d_color_border[3]), font_dec->d_width_border);
				}
				string->retain();
				LabelDetail * l_de = new LabelDetail(string, font_dec->idx);

				InsertFontByName(font_dec->name, l_de);
			}
#ifdef USE_ARK_ARCHIVE
			IsUseArk = false;
			g_IsUseArk = IsUseArk;
#endif
		}

		///////////////////////////////////////////////////////////////////////////////
		void ObjectTexture::LoadFont()
		{
			p_listFont.clear();
			size_t numberfont = XMLMgr->GetNumberFont();
			for (size_t i = 0; i < numberfont; i++)
			{
				auto font_dec = XMLMgr->GetFontDecByIdx((int)i);
				Label* string = NULL;
				string = Label::createWithTTF(
					"",
					File::ResolveNamePath(font_dec->path).GetString(),
					font_dec->d_size
				);
				string->setColor(Color3B(font_dec->d_color[0], font_dec->d_color[1], font_dec->d_color[2]));
				string->setOpacity(font_dec->d_color[3]);
				if (font_dec->d_width_border > 0)
				{
					string->enableOutline(Color4B(font_dec->d_color_border[0], font_dec->d_color_border[1], font_dec->d_color_border[2], font_dec->d_color_border[3]), font_dec->d_width_border);
				}
				string->retain();
				LabelDetail * l_de = new LabelDetail(string, font_dec->idx);

				InsertFontByName(font_dec->name, l_de);
			}
		}

	}
}