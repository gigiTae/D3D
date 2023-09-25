#pragma once

#include "Mesh.h"

/// <summary>
/// 측지구 
/// 
/// 면적이 거의 같이고 변의 길이도 같은 삼각형들로 구를 근사한다.
/// 
/// </summary>
class GeoSphere :Mesh
{
public:
	GeoSphere(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs);
	~GeoSphere();

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT3 tangentU;
		XMFLOAT2 texC;
	};
	
public:
	void Initilize(float radius, UINT numSubdivisions);
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
	void Render();

private:
	void BulidBuffers(float radius, UINT numSubdivisions);
	void BuildEffect();
	void BuildLayout();

	void SubDivide(std::vector<Vertex>& vertices, std::vector<UINT>& indices);

private:
	UINT m_vertexSize;
	UINT m_indexSize;
	UINT m_numSubdivisions;
	float m_radius;

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
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorld; // 월드 
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorldInvTranspose; // 월드역전치
	ComPtr<ID3DX11EffectVectorVariable> m_fxEyePosW; // eye

	ComPtr<ID3DX11EffectVariable> m_fxDirLight;
	ComPtr<ID3DX11EffectVariable> m_fxPointLight;
	ComPtr<ID3DX11EffectVariable> m_fxSpotLight;
	ComPtr<ID3DX11EffectVariable> m_fxMaterial;


};

