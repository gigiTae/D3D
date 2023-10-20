#include "RendererPCH.h"
#include "InputLayout.h"

RendererModule::InputLayout::InputLayout()
	:m_simple(), m_basic(), m_basicDesc()
{
	m_simpleDesc[0] = { "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_simpleDesc[1] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	m_basicDesc[0] = { "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,
		0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_basicDesc[1] = { "NORMAL", 0,DXGI_FORMAT_R32G32B32_FLOAT,
		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	m_basicDesc[2] = { "TEXCOORD", 0,DXGI_FORMAT_R32G32_FLOAT,
		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 };

	/// 아직 사용 X
	/*m_basicDesc[3] = { "POSITION", 0,DXGI_FORMAT_R32G32B32_FLOAT,
			0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 };*/
}

RendererModule::InputLayout::~InputLayout()
{

}

void RendererModule::InputLayout::Initailize(ID3D11Device* device)
{
	m_d3dDevice = device;



}

void RendererModule::InputLayout::Finalize()
{

}
