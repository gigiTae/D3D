#pragma once

namespace GrapicsEngine
{
	class Effect;

	/// <summary>
	/// ����Ʈ�� �����ϰ� �����ϴ� Ŭ����
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

