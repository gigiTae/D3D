#pragma once

namespace RendererModule
{

	// 물질의 특성을 정한다 
class Material
{
public:
	Material();
	~Material();

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT4 reflect;
};


}