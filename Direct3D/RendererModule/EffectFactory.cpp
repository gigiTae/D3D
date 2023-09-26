#include "RendererPCH.h"
#include "EffectFactory.h"
#include "Effect.h"

GrapicsEngine::EffectFactory::EffectFactory()
	:m_effects{}
{

}

GrapicsEngine::EffectFactory::~EffectFactory()
{

}

void GrapicsEngine::EffectFactory::Initalize(ID3D11Device* device)
{
	/// 이펙트들 생성 
	Effect* basicFX = new BasicEffect(device, L"..\\Resource\\Shader\\Basic.cso");
	m_effects.insert(std::make_pair(L"Basic", basicFX));
}

GrapicsEngine::Effect* GrapicsEngine::EffectFactory::GetEffect(const std::wstring& name)
{
	auto iter = m_effects.find(name);
	assert(iter != m_effects.end());

	return iter->second.get();
}
