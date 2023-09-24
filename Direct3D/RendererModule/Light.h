#pragma once

// Ambient 광원의 방출하는 주변광의 양
// Diffuse 광원이 방출하는 분산광의 양
// Specular 광원이 방출하는 반영광의 양
// Direction 빛의 방향
// Position 광원의 위치
// Range 빛의 범위, 광원과의 거리가 이 범위보다 큰 점은 빛을 받지 않는다
// Attenuation 빛의 세기가 거리에 따라 감소하는 방식을 제어하는 세 개의 감쇠 상수들을 (a0,a1,a2) 의형태
// Spot 점적광 계산에서 점적광 원뿔을 제어하는데 쓰이는 지수 


struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient; 
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	float pad; // 필요하다면 빛들의 배열을 설정할 수 있도록, 마지막에 float 하나의 자리를 마련해둔다.
};


struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Position, Range 형태로 하나의 4차원 벡터에 채워 넣는다.
	XMFLOAT3 position;
	float range;

	// (A0,A1,A2,Pad) 형태로 하나의 4차원 벡터에 채워 넣는다.
	XMFLOAT3 att;
	float pad; 
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Position, Range 형태로 하나의 4차원 벡터에 채워 넣는다.
	XMFLOAT3 position;
	float range;

	// Direction, Spot 형태로 하나의 4차원 벡터에 채워넣는다.
	XMFLOAT3 direction;
	float spot;

	// (A0,A1,A2,Pad) 형태로 하나의 4차원 벡터에 채워 넣는다.
	XMFLOAT3 att;
	float pad;

}; 


struct Material
{
	Material() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular; // w = SpecPower
	XMFLOAT4 reflect;
};