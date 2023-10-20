#pragma once


namespace RendererModule
{
	class Effect;
}

namespace RendererModule
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

	public:
		ID3D11ShaderResourceView* GetShaderResourceView(const std::wstring& fileName);
		RendererModule::Effect* GetEffect(const std::wstring& effectKey);

	private:
		void LoadTextures();
		void LoadEffect();

	private:
     	void CreateShaderResourceView(const std::wstring& filePath);

	private:
		ComPtr<ID3D11Device> m_d3dDevice;
		std::unordered_map<std::wstring, ComPtr<ID3D11ShaderResourceView>> m_shaderResourceViewMap;
		std::unordered_map<std::wstring, std::unique_ptr<RendererModule::Effect>> m_effects;
	};

}