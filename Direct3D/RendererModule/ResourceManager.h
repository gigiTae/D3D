#pragma once

namespace GrapicsEngine
{
	/// <summary>
	/// ���ҽ����� ����ϴ� Ŭ���� 
	/// 
	/// </summary>
	class ResourceManager
	{
	public:
		ResourceManager();
		~ResourceManager();
		
		void Initialize(ID3D11Device* device);


	private:
		std::map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_shaderResourceViewMap;
	};


}