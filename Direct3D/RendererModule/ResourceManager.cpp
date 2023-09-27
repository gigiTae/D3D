#include "RendererPCH.h"
#include "ResourceManager.h"
#include "FileLoader.h"


GrapicsEngine::ResourceManager::ResourceManager()
{

}

GrapicsEngine::ResourceManager::~ResourceManager()
{

}

void GrapicsEngine::ResourceManager::Initialize(ID3D11Device* device)
{
	ID3D11Resource* texResource = nullptr;
	ID3D11ShaderResourceView* m_resourceView = nullptr;
	const std::wstring& filePath = GrapicsEngine::File::GetCurrentFilePath();

	// 텍스처 파일경로 불러오기 
	std::wstring texturePath = filePath + L"Resource\\Texture\\box.bmp";	

	
	CreateShaderResourceView(device, texturePath);
}

void GrapicsEngine::ResourceManager::CreateShaderResourceView(ID3D11Device* device, const std::wstring& filePath)
{
	// 키값 생성
	wstring fileName = File::GetFileName(filePath);

	auto iter = m_textureMap.find(fileName);
	assert(iter == m_textureMap.end() || !L"중복 키값입니다.");

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
	HR(CreateTexture(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(),
		reinterpret_cast<ID3D11Resource**>(&texture)));

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Format = image.GetMetadata().format;

	ID3D11ShaderResourceView* shaderResourceView = nullptr;
	HR(device->CreateShaderResourceView(texture, &srvDesc, &shaderResourceView));

	texture->Release();

	m_textureMap.insert(std::make_pair(fileName, shaderResourceView));
}
