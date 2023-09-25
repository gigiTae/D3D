#include "RendererPCH.h"
#include "Land.h"

Land::Land(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device), m_d3dDeviceContext(deviceContext), m_rasterizerState(rs)
	, m_proj(), m_world(), m_view(), m_indexBuffer(), m_vertexBuffer(), m_inputLayout()
	, m_vertexSize(0), m_indexSize(0)
{
	m_landColor = XMFLOAT4(1.f, 1.f, 1.f, 1.f);


	m_landMaterial.ambient = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	m_landMaterial.diffuse = XMFLOAT4(0.48f, 0.77f, 0.46f, 1.0f);
	m_landMaterial.specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);
}

Land::~Land()
{

}

void Land::Initialize(float width, float depth, UINT xVertex, UINT zVertex)
{
	BuildBuffers(width, depth, xVertex, zVertex);
	BuildEffect();
	BuildLayout();
}

void Land::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void Land::Render(DirectionalLight* dirLight, PointLight* pointLight
	, SpotLight* spotLight, XMFLOAT3* eyePosition)
{
	// 입력 배치 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// 버텍스 버퍼와 인덱스 버퍼 셋팅
	UINT stride = sizeof(Land::Vertex);
	UINT offset = 0;

	m_d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// Set per object constants.
	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX worldInvTranspose = DM::InverseTranspose(world);

	// 행렬
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX worldViewProj = world * view * proj;


	// 상수 버퍼 변수를 통해서 월드뷰프로젝션 행렬을 셋팅해준다.

	m_fxWorld->SetMatrix(reinterpret_cast<float*>(&world));
	m_fxWorldInvTranspose->SetMatrix(reinterpret_cast<float*>(&worldInvTranspose));
	m_fxMaterial->SetRawValue(&m_landMaterial, 0, sizeof(m_landMaterial));

	m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));
	SetRawValue(dirLight, pointLight, spotLight, eyePosition);

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	// 랜더패스?
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_d3dDeviceContext.Get());
		m_d3dDeviceContext->DrawIndexed(m_indexSize, 0, 0);
	}
}

void Land::SetRawValue(DirectionalLight* dirLight, PointLight* pointLight, SpotLight* spotLight, XMFLOAT3* eyePosition)
{
	m_fxDirLight->SetRawValue(dirLight, 0, sizeof(&dirLight));
	m_fxPointLight->SetRawValue(pointLight, 0, sizeof(&pointLight));
	m_fxSpotLight->SetRawValue(spotLight, 0, sizeof(&spotLight));
	m_fxEyePosW->SetRawValue(eyePosition, 0, sizeof(&eyePosition));

}

void Land::BuildBuffers(float width, float depth, UINT xVertex, UINT zVertex)
{
	UINT vertexCount = xVertex * zVertex;
	m_vertexSize = vertexCount;
	UINT faceCount = (xVertex - 1) * (zVertex - 1) * 2;

	// 정점을 만든다
	float halfWidth = 0.5f * width;
	float halfDepth = 0.5f * depth;

	float dx = width / (zVertex - 1);
	float dz = depth / (xVertex - 1);

	float du = 1.0f / (zVertex - 1);
	float dv = 1.0f / (xVertex - 1);

	// 정점버퍼 
	std::vector<Land::Vertex> v(vertexCount);

	for (UINT i = 0; i < xVertex; ++i)
	{
		float z = halfDepth - (i * dz);
		for (UINT j = 0; j < zVertex; ++j)
		{
			float x = -halfWidth + (j * dx);
			v[i * zVertex + j].position = XMFLOAT3(x, 0.0f, z);
			v[i * zVertex + j].color = m_landColor;

			// 조명에 쓰이는 특성
			v[i* zVertex + j].normal = XMFLOAT3(0.f, 1.f, 0.0f);

			// 텍스처 적용에 쓰이는 특성
		}
	}

	std::vector<UINT> index(faceCount * 3);

	UINT k = 0;
	for (UINT i = 0; i < xVertex - 1; ++i)
	{
		for (UINT j = 0; j < zVertex - 1; ++j)
		{
			index[k] = i * zVertex + j;
			index[k + 1] = i * zVertex + j + 1;
			index[k + 2] = (i + 1) * zVertex + j;
			index[k + 3] = (i + 1) * zVertex + j;
			index[k + 4] = i * zVertex + j + 1;
			index[k + 5] = (i + 1) * zVertex + j + 1;

			k += 6; // 다음 낱같은으로 
		}
	}

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(Land::Vertex) * v.size(); // 생성할 정점 버퍼의 크기
	BF.Usage = D3D11_USAGE_DEFAULT;  // 버퍼가 쓰이는 방식 
	BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점 버퍼
	BF.CPUAccessFlags = 0;
	BF.MiscFlags = 0;
	BF.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = v.data();

	// 생성한 버퍼를 저장한다
	HR(m_d3dDevice->CreateBuffer(&BF, &SD, m_vertexBuffer.GetAddressOf()));

	// 색인과 색인 버퍼 생성

	// 색인 버퍼를 서술하는 구조체를 채운다.
	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = static_cast<size_t>(sizeof(UINT) * index.size());
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	m_indexSize = static_cast<UINT>(index.size());

	// 색인 버퍼를 초기화할 자료를 지정한다.
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = index.data();

	// 색인 버퍼를 생성한다.
	HR(m_d3dDevice->CreateBuffer(&ibd, &initData, m_indexBuffer.GetAddressOf()));

}

void Land::BuildEffect()
{
	std::ifstream fin("..\\Resource\\Shader\\Lighting.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_d3dDevice.Get(), m_effect.GetAddressOf()));

	m_tech = m_effect->GetTechniqueByName("LightTech");
	m_fxWorldViewProj = m_effect->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_fxWorld = m_effect->GetVariableByName("gWorld")->AsMatrix();
	m_fxWorldInvTranspose = m_effect->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_fxEyePosW = m_effect->GetVariableByName("gEyePosW")->AsVector();
	m_fxDirLight = m_effect->GetVariableByName("gDirLight");
	m_fxPointLight = m_effect->GetVariableByName("gPointLight");
	m_fxSpotLight = m_effect->GetVariableByName("gSpotLight");
	m_fxMaterial = m_effect->GetVariableByName("gMaterial");
}

void Land::BuildLayout()
{
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_d3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_inputLayout.GetAddressOf()));

}
