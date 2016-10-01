#include "Effect.h"
#include "LoadDataXML.h"
using namespace cocos2d;
namespace Utility
{
	namespace Effect
	{
		void AddEffectWithMaterial(cocos2d::Node * object, EFFECT_TYPE type)
		{
			if (type == EFFECT_NONE)
				return;

			RKString urlString = XMLMgr->GetMaterialByName("effect")->GetUrlString();
			auto properties = Properties::createNonRefCounted(File::ResolveNamePath(urlString).GetString());

			Material *mat1 = Material::createWithProperties(properties);

			if (type == EFFECT_BLUR)
			{
				object->setGLProgramState(mat1->getTechniqueByName("blur")->getPassByIndex(0)->getGLProgramState());
			}
			else if (type == EFFECT_BLOOM)
			{
				object->setGLProgramState(mat1->getTechniqueByName("bloom")->getPassByIndex(0)->getGLProgramState());
			}
			else if (type == EFFECT_EDGE)
			{
				object->setGLProgramState(mat1->getTechniqueByName("edge_detect")->getPassByIndex(0)->getGLProgramState());
			}
			else if (type == EFFECT_OUTLINE)
			{
				object->setGLProgramState(mat1->getTechniqueByName("outline")->getPassByIndex(0)->getGLProgramState());
			}
		}

		void AddEffectWithShader(cocos2d::Node * object, EFFECT_TYPE type, FLATFORM fl)
		{
			if (type == EFFECT_NONE)
				return;
			RKString vert_url = "";
			RKString pixel_url = "";
			xml::ShaderDec* _shader = NULL;
			if (type == EFFECT_BLUR)
			{
				_shader = XMLMgr->GetShaderByName("blur");
			}
			else if (type == EFFECT_BLOOM)
			{
				_shader = XMLMgr->GetShaderByName("bloom");
			}
			else if (type == EFFECT_EDGE)
			{
				_shader = XMLMgr->GetShaderByName("edge_detect");
			}
			else if (type == EFFECT_OUTLINE)
			{
				_shader = XMLMgr->GetShaderByName("outline");
			}

			if (fl == FLATFORM::HLSL)
			{
				vert_url = _shader->p_name_vert_dx;
				pixel_url = _shader->p_name_pixel_dx;
			}
			else if (fl == FLATFORM::GLSL)
			{
				vert_url = _shader->p_name_vert_gl;
				pixel_url = _shader->p_name_pixel_gl;
			}
			//
			auto fragSource = FileUtils::getInstance()->getStringFromFile(File::ResolveNamePath(pixel_url).GetString());
			auto vertSource = FileUtils::getInstance()->getStringFromFile(File::ResolveNamePath(vert_url).GetString());

			auto glprogram = GLProgram::createWithByteArrays(vertSource.c_str(), fragSource.c_str());
			auto glprogramstate = GLProgramState::getOrCreateWithGLProgram(glprogram);

			object->setGLProgramState(glprogramstate);
			object->getGLProgramState()->setUniformVec2("resolution", Vec2(100,100));
			object->getGLProgramState()->setUniformVec2("center", Vec2(50, 50));
		}

	}
}