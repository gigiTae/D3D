   #pragma once

namespace RendererModule
{
	class Material;
	class DirectionalLight;
}


namespace RendererModule
{
#pragma region Effect
	class Effect
	{
	public:
		Effect(ID3D11Device* device, const std::wstring& fileName);
		virtual ~Effect();

	private:
		// 복사생성을 금지 
		Effect(const Effect& rhs);
		Effect& operator=(const Effect& rhs);

	protected:
		ComPtr<ID3DX11Effect> m_fx;
	};
#pragma endregion


}


