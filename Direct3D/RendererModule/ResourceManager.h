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
     	void CreateShaderResourceView(ID3D11Device* device, const std::wstring& filePath);

	private:
		std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_textureMap;

	};


}