#include "RendererPCH.h"
#include "Effect.h"
#include "Light.h"
#include "Material.h"


GrapicsEngine::Effect::Effect(ID3D11Device* device, const std::wstring& fileName)
{
	/// 쉐이더 불러오기 
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiledShader(size);

	fin.read(&compiledShader[0], size);
	fin.close();

	HR(D3DX11CreateEffectFromMemory(&compiledShader[0], size,
		0, device, &m_fx));
}

GrapicsEngine::Effect::~Effect()
{

}

GrapicsEngine::BasicEffect::BasicEffect(ID3D11Device* device, const std::wstring& fileName)
	:Effect(device, fileName), m_light1Tech(nullptr),m_light2Tech(nullptr),m_light3Tech(nullptr)
	,m_light0TexTech(nullptr),m_light1TexTech(nullptr),m_light2TexTech(nullptr)
	,m_light3TexTech(nullptr),m_worldViewProj(nullptr),m_world(nullptr)
	,m_worldInvTranspose(nullptr),m_texTransform(nullptr),m_eyePosW(nullptr)
	,m_dirLights(nullptr),m_material(nullptr),m_diffuseMap(nullptr)
{
	m_light1Tech = m_fx->GetTechniqueByName("Light1");
	m_light2Tech = m_fx->GetTechniqueByName("Light2");
	m_light3Tech = m_fx->GetTechniqueByName("Light3");
	
	m_light0TexTech = m_fx->GetTechniqueByName("Light0Tex");
	m_light1TexTech = m_fx->GetTechniqueByName("Light1Tex");
	m_light2TexTech = m_fx->GetTechniqueByName("Light2Tex");
	m_light3TexTech = m_fx->GetTechniqueByName("Light3Tex");
	
	m_worldViewProj = m_fx->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_world = m_fx->GetVariableByName("gWorld")->AsMatrix();
	m_worldInvTranspose = m_fx->GetVariableByName("gWorldInvTranspose")->AsMatrix();
	m_texTransform = m_fx->GetVariableByName("gTexTransform")->AsMatrix();
	m_eyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
	m_dirLights = m_fx->GetVariableByName("gDirLights");
	m_material = m_fx->GetVariableByName("gMaterial");
	m_diffuseMap = m_fx->GetVariableByName("gDiffuseMap")->AsShaderResource();
}

GrapicsEngine::BasicEffect::~BasicEffect()
{

}

void GrapicsEngine::BasicEffect::SetDirLights(const DirectionalLight* lights)
{
	m_dirLights->SetRawValue(lights, 0, 3 * sizeof(DirectionalLight));
}

void GrapicsEngine::BasicEffect::SetMaterial(const Material& mat)
{
	m_material->SetRawValue(&mat, 0, sizeof(Material));
}
