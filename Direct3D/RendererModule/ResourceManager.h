#pragma once

namespace GrapicsEngine
{
	/// <summary>
	/// 리소스들을 담당하는 클래스 
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