#pragma once

namespace GrapicsEngine
{
	class Effect;

	/// <summary>
	/// 이펙트를 생성하고 관리하는 클래스
	/// 
	/// </summary>
	class EffectFactory
	{
	public:
		EffectFactory(ID3D11Device* device);
		~EffectFactory();

		Effect* GetEffect(const  std::wstring& name);

	private:
		std::map<std::wstring, std::unique_ptr<Effect>> m_effects; 
	};
}

