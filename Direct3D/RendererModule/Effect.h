   #pragma once

namespace GrapicsEngine
{
	class Material;
	class DirectionalLight;
}


namespace GrapicsEngine
{
#pragma region Effect
	class Effect
	{
	public:
		Effect(ID3D11Device* device, const std::wstring& fileName);
		virtual ~Effect();

	private:
		// ��������� ���� 
		Effect(const Effect& rhs);
		Effect& operator=(const Effect& rhs);

	protected:
		ComPtr<ID3DX11Effect> m_fx;
	};
#pragma endregion


}

