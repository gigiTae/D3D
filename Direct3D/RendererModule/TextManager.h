#pragma once

namespace RendererModule
{
	/// <summary>
	/// text 출력을 도와주는 매니져
	/// 
	/// </summary>
	class TextManager
	{
	public:
		TextManager();
		~TextManager();

		void Initialize(ID3D11Device* device, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds);

		void DrawTextColor(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, const std::wstring& text);

	private:
		std::unique_ptr<SpriteBatch> m_spriteBatch;
		std::unique_ptr<SpriteFont> m_spriteFont;

		ComPtr< ID3D11RasterizerState> m_rasterizerState;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;
	};
}

