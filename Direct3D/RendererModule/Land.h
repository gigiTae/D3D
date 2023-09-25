#pragma once
#include "Mesh.h"
#include "Light.h"
#include "Material.h"

struct DirectionalLight;
struct PointLight;
struct SpotLight;

/// <summary>
/// ��
/// 
/// </summary>
class Land :
    public Mesh
{
public:
    Land(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs);
    ~Land();

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT3 normal;
        XMFLOAT4 color;
    };
    
public:
    void Initialize(float width, float depth, UINT xVertex, UINT zVertex);
	void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj);
	void Render(DirectionalLight* dirLight, PointLight* pointLight
		, SpotLight* spotLight, XMFLOAT3* eyePosition);
	void SetRawValue(DirectionalLight* dirLight, PointLight* pointLight
		, SpotLight* spotLight, XMFLOAT3* eyePosition);

private:
	void BuildBuffers(float width, float depth, UINT xVertex, UINT zVertex);
	void BuildEffect();
	void BuildLayout();

private:
	unsigned int m_vertexSize;
	unsigned int m_indexSize;
	XMFLOAT4 m_landColor;
	Material m_landMaterial;

	XMFLOAT4X4 m_world; // ���� ��ȯ ���
	XMFLOAT4X4 m_view;  // �þ� ��ȯ ���
	XMFLOAT4X4 m_proj;  // ���� ��ȯ ��� 

	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;

	/// ======================== �ڿ� =============================
	ComPtr<ID3D11Buffer>  m_vertexBuffer;
	ComPtr<ID3D11Buffer>  m_indexBuffer;
	ComPtr<ID3D11InputLayout> m_inputLayout;
	ComPtr<ID3D11RasterizerState> m_rasterizerState;

	ComPtr<ID3DX11Effect> m_effect;	// ����Ʈ
	ComPtr<ID3DX11EffectTechnique> m_tech;	// ��ũ
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorldViewProj;	// ����Ʈ�뺯��
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorld; // ���� 
	ComPtr<ID3DX11EffectMatrixVariable> m_fxWorldInvTranspose; // ���忪��ġ
	ComPtr<ID3DX11EffectVectorVariable> m_fxEyePosW; // eye
	
	ComPtr<ID3DX11EffectVariable> m_fxDirLight;
	ComPtr<ID3DX11EffectVariable> m_fxPointLight;
	ComPtr<ID3DX11EffectVariable> m_fxSpotLight;
	ComPtr<ID3DX11EffectVariable> m_fxMaterial;



};

