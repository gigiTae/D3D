#include "RendererPCH.h"
#include "TextManager.h"

RendererModule::TextManager::TextManager()
	: m_spriteBatch(nullptr), m_spriteFont(nullptr), m_rasterizerState(nullptr), m_depthStencilState(nullptr)
{

}

RendererModule::TextManager::~TextManager()
{

}

void RendererModule::TextManager::Initialize(ID3D11Device* device, ID3D11RasterizerState* rs, ID3D11DepthStencilState* ds)
{
	ID3D11DeviceContext* deviceContext = nullptr;
	device->GetImmediateContext(&deviceContext);

	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(deviceContext);
	

	/// 폰트생성 
	std::wstring fileName = L"../Resource/Font/gulim9k.spritefont";

	m_spriteFont = std::make_unique<SpriteFont>(device, fileName.c_str());
	m_spriteFont->SetLineSpacing(10.f);

	deviceContext->Release();

	m_rasterizerState = rs;
	m_depthStencilState = ds; 
}


void RendererModule::TextManager::DrawTextColor(DirectX::XMFLOAT2 position, DirectX::XMFLOAT4 color, const std::wstring& text)
{
	m_spriteBatch->Begin(SpriteSortMode::SpriteSortMode_Deferred, nullptr, nullptr, m_depthStencilState.Get(), m_rasterizerState.Get());

	m_spriteFont->DrawString(m_spriteBatch.get(), text.c_str(), position, XMVectorSet(color.x, color.y, color.z, color.w));

	m_spriteBatch->End();
}

