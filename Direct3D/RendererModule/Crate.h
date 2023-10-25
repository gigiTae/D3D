#pragma once
#include "Mesh.h"
#include "Material.h"

namespace RendererModule
{
class BasicEffect;
}

namespace RendererModule
{
	/// <summary>
	/// 텍스처를 가진 박스
	/// </summary>
class Crate :
	public Mesh
{
public:
	Crate(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs
		  , ID3D11InputLayout* inputLayout, BasicEffect* effect);
	~Crate();

public:
	void Initialize(float width, float height, float depth);
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
	void Render();

private:
	void BuildBuffers(float width, float height, float depth);


private:
	int m_vertexSize;
	int m_indexSize;

	XMFLOAT4X4 m_world; // 월드 변환 행렬
	XMFLOAT4X4 m_view;  // 시야 변환 행렬
	XMFLOAT4X4 m_proj;  // 투명 변환 행렬 

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;

	ComPtr<ID3D11Buffer>  m_vertexBuffer;
	ComPtr<ID3D11Buffer>  m_indexBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;


	BasicEffect* m_basicEffect;
	Material m_material;

};


}