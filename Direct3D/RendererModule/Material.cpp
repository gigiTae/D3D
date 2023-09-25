#include "RendererPCH.h"
#include "Material.h"


Material::Material()
	:ambient(), diffuse(), specular(), reflect()
{
	 ZeroMemory(this, sizeof(this)); 
}

Material Material::GetLandMaterial()
{
	Material land;

	land.ambient = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	land.diffuse = XMFLOAT4(0.137f, 0.42f, 0.556f, 1.0f);
	land.specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

	return std::move(land);
}