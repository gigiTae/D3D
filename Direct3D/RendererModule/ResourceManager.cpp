#include "RendererPCH.h"
#include "ResourceManager.h"
#include "FileLoader.h"


GrapicsEngine::ResourceManager::ResourceManager()
	:m_d3dDevice()
	,m_textureMap{}
{

}

GrapicsEngine::ResourceManager::~ResourceManager()
{

}

void GrapicsEngine::ResourceManager::Initialize(ID3D11Device* device)
{
	m_d3dDevice = device;

	/// �ؽ�ó �ε� 
	LoadTextures();

}

ID3D11ShaderResourceView* GrapicsEngine::ResourceManager::GetShaderResourceView(const wstring& fileName)
{
	
}

void GrapicsEngine::ResourceManager::LoadTextures()
{
	// �ؽ�ó ���ϰ�� �ҷ����� 
	const std::wstring& filePath = GrapicsEngine::File::GetCurrentFilePath();
	std::wstring texturePath = filePath + L"Resource\\Texture";

	vector<wstring> textureFileMember{};

	File::GetFileMemberPath(textureFileMember, texturePath, true);

	for (const auto& path : textureFileMember)
	{
		CreateShaderResourceView(path);
	}
}

void GrapicsEngine::ResourceManager::CreateShaderResourceView(const std::wstring& filePath)
{
	wstring fileName = File::GetFileName(filePath); // fileName => key

	auto iter = m_textureMap.find(fileName);
	assert(iter == m_textureMap.end() || !L"�ߺ� Ű���Դϴ�.");

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

	assert(hr == S_OK || !L"���Ϸε� ����");

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

	m_textureMap.insert(std::make_pair(fileName, shaderResourceView));
}
