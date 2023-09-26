#include "RendererPCH.h"
#include "EffectFactory.h"
#include "Effect.h"

GrapicsEngine::EffectFactory::EffectFactory(ID3D11Device* device)
	:m_effects{}
{
	/// 이펙트들 생성 
	Effect* basicFX = new BasicEffect(device, L"..\\Resource\\Shader\\Basic.cso");
	m_effects.insert(std::make_pair(L"Basic", basicFX));
}

GrapicsEngine::EffectFactory::~EffectFactory()
{

}

GrapicsEngine::Effect* GrapicsEngine::EffectFactory::GetEffect(const std::wstring& name)
{
	auto iter = m_effects.find(name);
	assert(iter != m_effects.end());

	return iter->second.get();
}
