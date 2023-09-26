#pragma once

namespace GrapicsEngine
{
	/// 라이트 관련 

	class DirectionalLight
	{
	public:
		DirectionalLight();
		~DirectionalLight();

		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT3 direction;
		float pad;
	};

	class PointLight
	{
	public:
		PointLight();
		~PointLight();

		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT3 position;
		float range;
		XMFLOAT3 att;
		float pad;
	};

	class SpotLight
	{
	public:
		SpotLight();
		~SpotLight();

		XMFLOAT4 ambient;
		XMFLOAT4 diffuse;
		XMFLOAT4 specular;
		XMFLOAT4 position;
		float range;
		XMFLOAT3 direction;
		float spot;
		XMFLOAT3 att;
		float pad;
	};
	

}