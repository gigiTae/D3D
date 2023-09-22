#include "RendererPCH.h"
#include "Sphere.h"

Sphere::Sphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device), m_d3dDeviceContext(deviceContext), m_rasterizerState(rs)
	, m_proj(), m_world(), m_view(), m_indexBuffer(), m_vertexBuffer(), m_inputLayout()
	, m_radius(0.f), m_sliceCount(0), m_stackCount(0), m_vertexSize(0), m_indexSize(0)
{

}

Sphere::~Sphere()
{

}

void Sphere::Initialize(float radius, UINT sliceCount, UINT stackCount)
{
	m_radius = m_radius;
	m_sliceCount = sliceCount;
	m_stackCount = stackCount;

	BuildBuffers(radius, sliceCount, stackCount);
	BuildEffect();
	BuildLayout();
}

void Sphere::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void Sphere::Render()
{
	// 입력 배치 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// 버텍스 버퍼와 인덱스 버퍼 셋팅
	UINT stride = sizeof(Sphere::Vertex);
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


void Sphere::BuildBuffers(float radius, UINT sliceCount, UINT stackCount)
{
	/// 정점정보 생성 

	// 맨 위에서 시작하여 스택 아래로 이동하는 정점을 계산한다.
	Sphere::Vertex topVertex{XMFLOAT3(0.0f, +radius, 0.0f), XMFLOAT3(.0f, +1.0f, 0.0f)
		,XMFLOAT3( 1.0f, 0.0f, 0.0f),XMFLOAT2( 0.0f, 0.0f)};

	Sphere::Vertex bottomVertex{ XMFLOAT3(0.0f, -radius, 0.0f),XMFLOAT3(0.0f, -1.0f, 0.0f)
		,XMFLOAT3(1.0f, 0.0f, 0.0f),XMFLOAT2(0.0f, 1.0f) };
	
	// 텍스처 좌표 왜곡이 발생하므로 주의
	std::vector<Sphere::Vertex> vertices{};
	std::vector<UINT> indices{};

	vertices.push_back(topVertex);

	float phiStep = XM_PI / stackCount;
	float thetaStep = 2.f * XM_PI / sliceCount;

	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i * phiStep;

		// Vertices of ring
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			Sphere::Vertex v;
			
			v.position.x = radius * sinf(phi) * cosf(theta);
			v.position.y = radius * cosf(phi);
			v.position.z = radius * sinf(phi) * sinf(theta);

			// Partial derivative of P with respect to theta
			v.tangentU.x = -radius * sinf(phi) * sinf(theta);
			v.tangentU.y = 0.0f;
			v.tangentU.z = +radius * sinf(phi) * cosf(theta);

			XMVECTOR T = XMLoadFloat3(&v.tangentU);
			XMStoreFloat3(&v.tangentU, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.position);
			XMStoreFloat3(&v.normal, XMVector3Normalize(p));

			v.texC.x = theta / XM_2PI;
			v.texC.y = phi / XM_PI;

			vertices.push_back(v);
		}

	}

	vertices.push_back(bottomVertex);

	/// 인덱스 정보 생성 


	/// 상단  
	for(UINT i=1; i<=sliceCount; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	/// 중간
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	/// 하단 연결
	UINT southPoleIndex = static_cast<UINT>(vertices.size()) - 1;

	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}


	/// 정점 인덱스 버퍼 생성
	m_indexSize = indices.size();
	m_vertexSize = vertices.size();

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(Sphere::Vertex) * m_vertexSize; // 생성할 정점 버퍼의 크기
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

void Sphere::BuildEffect()
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

void Sphere::BuildLayout()
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
