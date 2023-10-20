#include "RendererPCH.h"
#include "GeoSphere.h"

GeoSphere::GeoSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device), m_d3dDeviceContext(deviceContext), m_rasterizerState(rs)
	, m_proj(), m_world(), m_view(), m_indexBuffer(), m_vertexBuffer(), m_inputLayout()
	, m_numSubdivisions(0), m_radius(0.f), m_indexSize(0), m_vertexSize(0)
{

}

GeoSphere::~GeoSphere()
{

}

void GeoSphere::Initilize(float radius, UINT numSubdivisions)
{
	m_radius = radius;


	BulidBuffers(radius, numSubdivisions);
	BuildEffect();
	BuildLayout();
}

void GeoSphere::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void GeoSphere::Render()
{
	// 입력 배치 셋팅
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// 버텍스 버퍼와 인덱스 버퍼 셋팅
	UINT stride = sizeof(GeoSphere::Vertex);
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

void GeoSphere::BulidBuffers(float radius, UINT numSubdivisions)
{
	// 세분 횟수에 상한을 둔다
	numSubdivisions = std::min<UINT>(numSubdivisions, 5u);
	m_numSubdivisions = numSubdivisions;

	// 정이십면체를 테셀레이션해서 구를 근사한다.
	constexpr float X = 0.525731f;
	constexpr float Z = 0.850651f;

	XMFLOAT3 pos[12] =
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X),
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f),
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	std::vector<GeoSphere::Vertex> vertices{};
	std::vector<UINT> indices{};

	vertices.resize(12);
	indices.resize(60);

	for (UINT i = 0; i < 12; ++i)
		vertices[i].position = pos[i];

	for (UINT i = 0; i < 60; ++i)
		indices[i] = k[i];

	for (UINT i = 0; i < numSubdivisions; ++i)
		SubDivide(vertices, indices);

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < vertices.size(); ++i)
	{
		// Project onto unit sphere.
		XMVECTOR n = XMVector3Normalize(XMLoadFloat3(&vertices[i].position));

		// Project onto sphere.
		XMVECTOR p = radius * n;

		XMStoreFloat3(&vertices[i].position, p);
		XMStoreFloat3(&vertices[i].normal, n);

		// Derive texture coordinates from spherical coordinates.
		float theta = MathModule::AngleFromXY(
			vertices[i].position.x,
			vertices[i].position.z);

		float phi = acosf(vertices[i].position.y / radius);

		vertices[i].texC.x = theta / XM_2PI;
		vertices[i].texC.y = phi / XM_PI;

		// Partial derivative of P with respect to theta
		vertices[i].tangentU.x = -radius * sinf(phi) * sinf(theta);
		vertices[i].tangentU.y = 0.0f;
		vertices[i].tangentU.z = +radius * sinf(phi) * cosf(theta);

		XMVECTOR T = XMLoadFloat3(&vertices[i].tangentU);
		XMStoreFloat3(&vertices[i].tangentU, XMVector3Normalize(T));
	}

	m_indexSize = static_cast<UINT>(indices.size());
	m_vertexSize =  static_cast<UINT>(vertices.size());

	D3D11_BUFFER_DESC BF{};
	BF.ByteWidth = sizeof(GeoSphere::Vertex) * m_vertexSize; // 생성할 정점 버퍼의 크기
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

void GeoSphere::BuildEffect()
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

void GeoSphere::BuildLayout()
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

void GeoSphere::SubDivide(std::vector<Vertex>& vertices, std::vector<UINT>& indices)
{
	// Save a copy of the input geometry.
	// 복사
	std::vector<Vertex> verticesCopy = vertices;
	std::vector<UINT> indicesCopy = indices;

	vertices.resize(0);
	indices.resize(0);

	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	UINT numTris = static_cast<UINT>(indicesCopy.size() / 3);
	for (UINT i = 0; i < numTris; ++i)
	{
		Vertex v0 = verticesCopy[indicesCopy[i * 3 + 0]];
		Vertex v1 = verticesCopy[indicesCopy[i * 3 + 1]];
		Vertex v2 = verticesCopy[indicesCopy[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		Vertex m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.position = XMFLOAT3(
			0.5f * (v0.position.x + v1.position.x),
			0.5f * (v0.position.y + v1.position.y),
			0.5f * (v0.position.z + v1.position.z));

		m1.position = XMFLOAT3(
			0.5f * (v1.position.x + v2.position.x),
			0.5f * (v1.position.y + v2.position.y),
			0.5f * (v1.position.z + v2.position.z));

		m2.position = XMFLOAT3(
			0.5f * (v0.position.x + v2.position.x),
			0.5f * (v0.position.y + v2.position.y),
			0.5f * (v0.position.z + v2.position.z));

		//
		// Add new geometry.
		//

		vertices.push_back(v0); // 0
		vertices.push_back(v1); // 1
		vertices.push_back(v2); // 2
		vertices.push_back(m0); // 3
		vertices.push_back(m1); // 4
		vertices.push_back(m2); // 5

		indices.push_back(i * 6 + 0);
		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 5);

		indices.push_back(i * 6 + 5);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 2);

		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 1);
		indices.push_back(i * 6 + 4);
	}
}
