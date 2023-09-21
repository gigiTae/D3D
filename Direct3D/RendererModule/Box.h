#pragma once
#include "IMesh.h"

/// <summary>
/// �ڽ� �޽� 
/// 
/// 
/// </summary>
class Box :
    public IMesh
{
public:
    Box(ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RasterizerState* rs);
    ~Box();
    
    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };

public:
    void Initialize() override;
    void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj) override;
    void Render() override;

private:
    void BuildBuffers();
    void BuildFX();
    void BuildLayout();

private:
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

};

