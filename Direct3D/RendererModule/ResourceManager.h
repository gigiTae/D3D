#pragma once


namespace GrapicsEngine
{
	class Effect;
}

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
		ID3D11ShaderResourceView* GetShaderResourceView(const std::wstring& fileName);
		GrapicsEngine::Effect* GetEffect(const std::wstring& effectKey);

	private:
		void LoadTextures();
		void LoadEffect();

	private:
     	void CreateShaderResourceView(const std::wstring& filePath);

	private:
		ComPtr<ID3D11Device> m_d3dDevice;
		std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_shaderResourceViewMap;
		std::unordered_map<std::wstring, std::unique_ptr<GrapicsEngine::Effect>> m_effects;
	};

}