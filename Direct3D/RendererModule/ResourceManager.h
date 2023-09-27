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


	public:
		ID3D11ShaderResourceView* GetShaderResourceView(const wstring& fileName);

	private:
		void LoadTextures();

	private:
     	void CreateShaderResourceView(const std::wstring& filePath);

	private:
		std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_textureMap;
		ComPtr<ID3D11Device> m_d3dDevice;
	};


}