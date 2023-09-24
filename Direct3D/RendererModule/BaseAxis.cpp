#include "RendererPCH.h"
#include "BaseAxis.h"

BaseAxis::BaseAxis(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device), m_d3dDeviceContext(deviceContext), m_rasterizerState(rs)
	, m_proj(), m_world(), m_view(), m_indexBuffer(), m_vertexBuffer(), m_inputLayout()
	,m_vertexSize(0), m_indexSize()
{

}

BaseAxis::~BaseAxis()
{

}

void BaseAxis::Initalize()
{
	BuildBuffers();
	BuildEffect();
	BuildLayout();
}

void BaseAxis::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void BaseAxis::Render()
{
	// 입력 배치 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());

	/// 선을 그리기때문에 라인리스트 사용
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// 버텍스 버퍼와 인덱스 버퍼 셋팅
	UINT stride = sizeof(BaseAxis::Vertex);
	UINT offset = 0;

	m_d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// 행렬
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX worldViewProj = world * view * proj;

	// 상수 버퍼 변수를 통해서 월드뷰프로젝션 행렬을 셋팅해준다.
	m_fxWorldViewProj->SetMatrix(reinterpret_cast<float*>(&worldViewProj));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_tech->GetDesc(&techDesc);

	// 랜더패스?
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_d3dDeviceContext.Get());
		m_d3dDeviceContext->DrawIndexed(m_indexSize, 0, 0);
	}
}

void BaseAxis::BuildBuffers()
{
	constexpr XMFLOAT4 red{ 1.f,0.f,0.f,1.f };
	constexpr XMFLOAT4 blue { 0.f, 0.f, 1.f, 1.f };
	constexpr XMFLOAT4 green{ 0.f,1.f,0.f,1.f };

	/// 값이 너무 크면 문제가 발생하는듯 ?
	float fMax = 10000.f;//FLT_MAX;
	float fMin = -10000.f;//FLT_MIN;

	std::vector<BaseAxis::Vertex> vertices
	{

		// XAxis
		Vertex{XMFLOAT3(fMax, 0.f,0.f), red},
		Vertex{XMFLOAT3(fMin, 0.f,0.f), red},
		// YAxis
		Vertex{XMFLOAT3(0.f, fMax,0.f), green},
		Vertex{XMFLOAT3(0.f, fMin,0.f), green},
		// ZAxis
		Vertex{XMFLOAT3(0.f, 0.f,fMax),blue},
		Vertex{XMFLOAT3(0.f, 0.f,fMin),blue},
	};

	std::vector<UINT> indices
	{
		0,1,
		2,3,
		4,5
	};
	
	m_vertexSize = static_cast<UINT>(vertices.size());
	m_indexSize = static_cast<UINT>(indices.size());

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(BaseAxis::Vertex) * m_vertexSize; // 생성할 정점 버퍼의 크기
	BF.Usage = D3D11_USAGE_DEFAULT;  // 버퍼가 쓰이는 방식 
	BF.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점 버퍼
	BF.CPUAccessFlags = 0;
	BF.MiscFlags = 0;
	BF.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA SD{};
	SD.pSysMem = vertices.data();

	HR(m_d3dDevice->CreateBuffer(&BF, &SD, m_vertexBuffer.GetAddressOf()));
	

	// 색인 버퍼를 서술하는 구조체를 채운다.
	D3D11_BUFFER_DESC ibd{};
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_indexSize;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// 색인 버퍼를 초기화할 자료를 지정한다.
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indices.data();

	// 색인 버퍼를 생성한다.
	HR(m_d3dDevice->CreateBuffer(&ibd, &initData, m_indexBuffer.GetAddressOf()));
}

void BaseAxis::BuildEffect()
{
	std::ifstream fin("..\\Resource\\Shader\\color.cso", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = static_cast<int>(fin.tellg());
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, m_d3dDevice.Get(), m_effect.GetAddressOf()));

	m_tech = m_effect->GetTechniqueByName("ColorTech");
	m_fxWorldViewProj = m_effect->GetVariableByName("gWorldViewProj")->AsMatrix();
}

void BaseAxis::BuildLayout()
{
	// 인풋 레이아웃 설정
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	D3DX11_PASS_DESC passDesc;
	m_tech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_d3dDevice->CreateInputLayout(ied, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, m_inputLayout.GetAddressOf()));
}
