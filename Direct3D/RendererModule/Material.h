#pragma once



/// <summary>
/// �����Ӽ� 
/// </summary>
struct Material
{
	Material(); 

	static Material GetLandMaterial();

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
	XMFLOAT4 reflect;
};
