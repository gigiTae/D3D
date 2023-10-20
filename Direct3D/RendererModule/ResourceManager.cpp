#include "RendererPCH.h"
#include "ResourceManager.h"
#include "FileLoader.h"
#include "Effect.h"
#include "BasicEffect.h"


RendererModule::ResourceManager::ResourceManager()
	:m_d3dDevice()
	,m_shaderResourceViewMap{}
{

}

RendererModule::ResourceManager::~ResourceManager()
{

}

void RendererModule::ResourceManager::Initialize(ID3D11Device* device)
{
	m_d3dDevice = device;


	/// 텍스처 로딩 
	LoadTextures();

	/// 이펙트 로딩
	LoadEffect();
}

ID3D11ShaderResourceView* RendererModule::ResourceManager::GetShaderResourceView(const std::wstring& fileName)
{
	auto iter = m_shaderResourceViewMap.find(fileName);

	assert(iter != m_shaderResourceViewMap.end() || L"찾는 리소스가 없습니다.");

	return iter->second.Get();
}

RendererModule::Effect* RendererModule::ResourceManager::GetEffect(const std::wstring& effectKey)
{
	auto iter = m_effects.find(effectKey);
	assert(iter != m_effects.end());

	return iter->second.get();
}

void RendererModule::ResourceManager::LoadTextures()
{
	// 텍스처 파일경로 불러오기 
	const std::wstring& filePath = RendererModule::File::GetCurrentFilePath();
	std::wstring texturePath = filePath + L"Resource\\Texture";

	std::vector<std::wstring> textureFileMember{};

	File::GetFileMemberPath(textureFileMember, texturePath, true);

	for (const auto& path : textureFileMember)
	{
		CreateShaderResourceView(path);
	}

}

void RendererModule::ResourceManager::LoadEffect()
{
	/// 이펙트들 생성 
	Effect* basicFX = new BasicEffect(m_d3dDevice.Get(), L"..\\Resource\\Shader\\Basic.cso");
	m_effects.insert(std::make_pair(L"Basic", basicFX));

}

void RendererModule::ResourceManager::CreateShaderResourceView(const std::wstring& filePath)
{
	using std::wstring;

	wstring fileName = File::GetFileName(filePath); // fileName => key

	auto iter = m_shaderResourceViewMap.find(fileName);
	assert(iter == m_shaderResourceViewMap.end() || !L"중복 키값입니다.");

	wstring extension = File::GetFileExtension(filePath);

	DirectX::ScratchImage image;

	HRESULT hr = S_OK;

	if (extension == L"tga" || extension == L"targa")
	{
		hr = LoadFromTGAFile(filePath.c_str(), nullptr, image);
	}
	else
	{
		hr = LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, nullptr, image);
	}

	assert(hr == S_OK || !L"파일로드 실패");

	ID3D11Texture2D* texture = nullptr;
	HR(CreateTexture(m_d3dDevice.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		reinterpret_cast<ID3D11Resource**>(&texture)));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = image.GetMetadata().format;

	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	HR(m_d3dDevice->CreateShaderResourceView(texture, &srvDesc, &shaderResourceView));

	texture->Release();

	m_shaderResourceViewMap.insert(std::make_pair(fileName, shaderResourceView));
}
