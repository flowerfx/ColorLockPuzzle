#ifndef _OBJECT_TEXTURE_H_
#define _OBJECT_TEXTURE_H_

#include "math/CCGeometry.h"
#include "base/CCVector.h"
#include "cocos2d.h"
#include "renderer/CCTexture2D.h"
USING_NS_CC;
#define USE_SPRITE_STORE 1
#include "LoadDataXML.h"
using namespace RKUtils;
namespace Utility
{
	namespace Texture
	{
		struct LabelDetail
		{
			Label		* label;
			int			  idx;
		public:
			LabelDetail()
			{
				label = NULL;
				idx = 0;
			}
			LabelDetail(Label* l, int id)
			{
				label = l;
				idx = id;
			}
			virtual ~LabelDetail()
			{
				if (label)
				{
					delete label;
					label = NULL;
				}
				idx = -1;
			}
		};
		class ObjectTexture
		{
		public:
			void Load(bool loadAA = false);
			void LoadTexturePacker(RKString name, bool loadAA = false);
			void LoadFont();
			void UnloadData();

			Texture2D	*	GetTextureByName(RKString nameImage);
			Sprite		*	GetSpriteByName(RKString nameImage);
			Label		*	GetFontByName(RKString nameImage);
			Label		*	GetFontByIdx(int idx);

		protected:
			Texture2D * CreateTextureFromNamePath(RKString namepath, bool loadAA = false, int levelMipMap = 0);
			Sprite *	CreateSpriteFromTexture(Texture2D * tex, xml::AtlasChild* nameSprite, int levelMipMap = 0);

			void InsertTextureByName(RKString name, Texture2D * tex);
			void InsertSpriteByName(RKString name, Sprite * tex);
			void InsertFontByName(RKString name, LabelDetail * tex);
		private:

			//texture
			std::map<RKString, Texture2D *>			p_listTexture;
			
			//label for font
			std::map<RKString, LabelDetail * >		p_listFont;
			
			//sprite
			std::map<RKString, Sprite *>			p_listSprite;

			static ObjectTexture * p_Instance;

			int	p_levelMipMap;
		public:
			static ObjectTexture * GetInstance()
			{
				if (!p_Instance)
				{
					p_Instance = new ObjectTexture();
				}
				return p_Instance;
			}
			ObjectTexture();
			virtual ~ObjectTexture();

			int GetLevelMipMap() { return p_levelMipMap; }

		};
	}
#define TextureMgr Texture::ObjectTexture::GetInstance()

}
#endif