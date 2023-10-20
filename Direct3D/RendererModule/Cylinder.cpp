#include "RendererPCH.h"
#include "Cylinder.h"



Cylinder::Cylinder(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device), m_d3dDeviceContext(deviceContext), m_rasterizerState(rs)
	, m_proj(), m_world(), m_view(), m_indexBuffer(), m_vertexBuffer(), m_inputLayout()
	,indexSize(0), vertexSize(0),m_bottomRadius(0.f),m_topRadius(0.f),m_height(0.f)
	,m_sliceCount(0),m_stackCount(0)
{

}

Cylinder::~Cylinder()
{

}

void Cylinder::Initailize(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	m_bottomRadius = bottomRadius;
	m_topRadius = topRadius;
	m_height = height;
	m_sliceCount = sliceCount;
	m_stackCount = stackCount;

	BuildBuffers(bottomRadius,topRadius, height, sliceCount, stackCount);
	BuildEffect();
	BuildLayout();
}

void Cylinder::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void Cylinder::Render()
{
	// 입력 배치 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// 버텍스 버퍼와 인덱스 버퍼 셋팅
	UINT stride = sizeof(Cylinder::Vertex);
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
		m_d3dDeviceContext->DrawIndexed(indexSize, 0, 0);
	}
}


void Cylinder::BuildBuffers(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	// 정점, 인덱스 정보 생성 
	BuildClinderBody(bottomRadius, topRadius, height, sliceCount, stackCount, vertices, indices);
	BuildClinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, vertices, indices);
	BuildClinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, vertices, indices);

	// 정정, 인덱스 버퍼 생성 
	indexSize = indices.size();
	vertexSize = vertices.size();

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(Cylinder::Vertex) * vertexSize; // 생성할 정점 버퍼의 크기
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
	ibd.ByteWidth = sizeof(UINT) * indexSize;
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

void Cylinder::BuildEffect()
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

void Cylinder::BuildLayout()
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


void Cylinder::BuildClinderBody(float bottomRadius, float topRadius, float height
	, UINT sliceCount, UINT stackCount, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	// 더미들을 만든다.c
	float stackHegiht = height / stackCount;

	// 한 단계 위로 올라갈 때의 
	// 반지름 변화량을 구한다
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1;

	// 최하단 고리에서 최상단 고리까지 훓으면서 
	// 각 고리의 점정들을 계산한다.
	for (unsigned int i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHegiht;
		float r = bottomRadius + i * radiusStep;

		// 현재 고리의 정점들
		float dTheta = 2.f * MathModule::PI / sliceCount;
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			Cylinder::Vertex v{};
			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			v.position = DirectX::XMFLOAT3(r * c, y, r * s);
			v.color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

			vertices.push_back(std::move(v));
		}
	}

	// 원기둥에 텍스처를 제대로 입히려면 첫 정점과 마지막 정점이 중복되어야한다.
	// 이를 위해 정점개수를 조각 개수보다 하나 더 많게 한다.
	unsigned int ringVertexCount = sliceCount + 1;

	// 각 더미마다 색인들을 계산한다
	for (unsigned int i = 0; i < stackCount; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);

			indices.push_back(i * ringVertexCount + j);
			indices.push_back((i + 1) * ringVertexCount + j + 1);
			indices.push_back(i * ringVertexCount + j + 1);
		}
	}
}


void Cylinder::BuildClinderTopCap(float bottomRadius, float topRadius, float height
	, UINT sliceCount, UINT stackCount, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	UINT baseIndex = static_cast<UINT>(vertices.size());

	float y = 0.5f * height;
	float dTheta = 2.f * MathModule::PI / sliceCount;

	// 고리 정점들을 복제해서 마개 정점들을 만든다. 
	// (텍스처 좌표와 법선이 다르므로 이처럼 중복이 필요하다.)
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius * cosf(i * dTheta);
		float z = topRadius * sinf(i * dTheta);

		// 윈면 마개의 텍스처 좌표 면적이 밑면에 비례하도록, 
		// 텍스처 좌표를 높이에 따라 적절히 축소한다.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(Vertex{ XMFLOAT3(x,y,z),XMFLOAT4(0.f,1.f,0.f,1.f) });
	}

	// 마개의 중심 정점	
	vertices.push_back(Vertex{ XMFLOAT3(0.f,y,0.f),XMFLOAT4(0.f,1.f,0.f,1.f) });

	// 중심 정점의 색인 
	UINT centerIndex = static_cast<UINT>(vertices.size()) - 1;
	
	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}
}


void Cylinder::BuildClinderBottomCap(float bottomRadius, float topRadius, float height
	, UINT sliceCount, UINT stackCount, std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{

	UINT baseIndex = static_cast<UINT>(vertices.size());

	float y = -0.5f * height;
	float dTheta = 2.f * MathModule::PI / sliceCount;

	// 고리 정점들을 복제해서 마개 정점들을 만든다. 
	// (텍스처 좌표와 법선이 다르므로 이처럼 중복이 필요하다.)
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius * cosf(i * dTheta);
		float z = bottomRadius * sinf(i * dTheta);

		// 윈면 마개의 텍스처 좌표 면적이 밑면에 비례하도록, 
		// 텍스처 좌표를 높이에 따라 적절히 축소한다.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		vertices.push_back(Vertex{ XMFLOAT3(x,y,z),XMFLOAT4(0.f,1.f,0.f,1.f) });
	}

	// 마개의 중심 정점	
	vertices.push_back(Vertex{ XMFLOAT3(0.f,y,0.f),XMFLOAT4(0.f,1.f,0.f,1.f) });

	// 중심 정점의 색인 
	UINT centerIndex = static_cast<UINT>(vertices.size()) - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		indices.push_back(centerIndex);
		indices.push_back(baseIndex + i + 1);
		indices.push_back(baseIndex + i);
	}
}
