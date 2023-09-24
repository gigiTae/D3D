#pragma once
#include "Mesh.h"

/// <summary>
/// x,y,z 축을 그리는 메쉬
/// 
/// 
/// </summary>
class BaseAxis :
    public Mesh
{
public:
	BaseAxis(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs);
	~BaseAxis();

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	//enum class Axis
	//{
	//	XAxis,
	//	YAxis,
	//	ZAxis,
	//};

public:
	void Initalize();
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
	void Render();

private:
	void BuildBuffers();
	void BuildEffect();
	void BuildLayout();

private:
	UINT m_vertexSize;
	UINT m_indexSize;

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

