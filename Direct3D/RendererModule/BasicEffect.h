#pragma once
#include "Effect.h"

namespace RendererModule
{
class Material;
class DirectionalLight;
}

namespace RendererModule
{

class BasicEffect : public Effect
{
public:
	BasicEffect(ID3D11Device* device, const std::wstring& fileName);
	~BasicEffect();

	void SetWorldViewProj(CXMMATRIX M) { m_worldViewProj->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorld(CXMMATRIX M) { m_world->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetWorldInvTranspose(CXMMATRIX M) { m_worldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetTexTransform(CXMMATRIX M) { m_texTransform->SetMatrix(reinterpret_cast<const float*>(&M)); }
	void SetEyePosW(const XMFLOAT3& v) { m_eyePosW->SetRawValue(&v, 0, sizeof(XMFLOAT3)); }
	void SetDirLights(const DirectionalLight* lights);
	void SetMaterial(const Material& material);
	void SetDiffuseMap(ID3D11ShaderResourceView* tex) { m_diffuseMap->SetResource(tex); }

public:
	ID3DX11EffectTechnique* m_light1Tech;
	ID3DX11EffectTechnique* m_light2Tech;
	ID3DX11EffectTechnique* m_light3Tech;

	ID3DX11EffectTechnique* m_light0TexTech;
	ID3DX11EffectTechnique* m_light1TexTech;
	ID3DX11EffectTechnique* m_light2TexTech;
	ID3DX11EffectTechnique* m_light3TexTech;

private:
	ID3DX11EffectMatrixVariable* m_worldViewProj;
	ID3DX11EffectMatrixVariable* m_world;
	ID3DX11EffectMatrixVariable* m_worldInvTranspose;
	ID3DX11EffectMatrixVariable* m_texTransform;
	ID3DX11EffectVectorVariable* m_eyePosW;
	ID3DX11EffectVariable* m_dirLights;
	ID3DX11EffectVariable* m_material;

	ID3DX11EffectShaderResourceVariable* m_diffuseMap;
};

}