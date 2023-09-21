#include "RendererPCH.h"
#include "Box.h"

Box::Box(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs)
	:m_d3dDevice(device),m_d3dDeviceContext(deviceContext),m_rasterizerState(rs)
	,m_proj(),m_world(),m_view(),m_indexBuffer(),m_vertexBuffer(),m_inputLayout()
{}

Box::~Box()
{
	/// Conptr���
}

void Box::Initialize()
{
	BuildBuffers();
	BuildFX();
	BuildLayout();
}

void Box::Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj)
{
	XMStoreFloat4x4(&m_world, world);
	XMStoreFloat4x4(&m_view, view);
	XMStoreFloat4x4(&m_proj, proj);
}

void Box::Render()
{
	// �Է� ��ġ ����
	m_d3dDeviceContext->IASetInputLayout(m_inputLayout.Get());
	m_d3dDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_d3dDeviceContext->RSSetState(m_rasterizerState.Get());

	// ���ؽ� ���ۿ� �ε��� ���� ����
	UINT stride = sizeof(Box::Vertex);
	UINT offset = 0;

	m_d3dDeviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
	m_d3dDeviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	// ���


}

void Box::BuildBuffers()
{

}

void Box::BuildFX()
{

}

void Box::BuildLayout()
{

}
