#pragma once


/// <summary>
/// xz 평면 그리드 메쉬
/// </summary>
class Grid
{
public:
    Grid(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs);
    ~Grid();

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

public:
	void Initialize(float width, float depth,UINT xVertex, UINT zVertex);
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
	void Render();

private:
	void BuildBuffers(float width, float depth, UINT xVertex, UINT zVertex);
	void BuildEffect();
	void BuildLayout();

private:
	unsigned int m_vertexSize;
	unsigned int m_indexSize;

	XMFLOAT4 m_gridColor;
	XMFLOAT4X4 m_world; // 월드 변환 행렬
	XMFLOAT4X4 m_view;  // 시야 변환 행렬
	XMFLOAT4X4 m_proj;  // 투명 변환 행렬 

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;

	/// ======================== 자원 =============================
	ComPtr<ID3D11Buffer>  m_vertexBuffer;
	ComPtr<ID3D11Buffer>  m_indexBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	ComPtr<ID3DX11Effect> m_effect;	// 이펙트
	ComPtr<ID3DX11EffectTechnique> m_tech;	// 테크
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorldViewProj;	// 이펙트용변수
};

