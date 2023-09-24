#pragma once

// Ambient ������ �����ϴ� �ֺ����� ��
// Diffuse ������ �����ϴ� �л걤�� ��
// Specular ������ �����ϴ� �ݿ����� ��
// Direction ���� ����
// Position ������ ��ġ
// Range ���� ����, �������� �Ÿ��� �� �������� ū ���� ���� ���� �ʴ´�
// Attenuation ���� ���Ⱑ �Ÿ��� ���� �����ϴ� ����� �����ϴ� �� ���� ���� ������� (a0,a1,a2) ������
// Spot ������ ��꿡�� ������ ������ �����ϴµ� ���̴� ���� 


struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient; 
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	XMFLOAT3 direction;
	float pad; // �ʿ��ϴٸ� ������ �迭�� ������ �� �ֵ���, �������� float �ϳ��� �ڸ��� �����صд�.
};


struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(this)); }

	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Position, Range ���·� �ϳ��� 4���� ���Ϳ� ä�� �ִ´�.
	XMFLOAT3 position;
	float range;

	// (A0,A1,A2,Pad) ���·� �ϳ��� 4���� ���Ϳ� ä�� �ִ´�.
	XMFLOAT3 att;
	float pad; 
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(this)); }
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT4 specular;
	// Position, Range ���·� �ϳ��� 4���� ���Ϳ� ä�� �ִ´�.
	XMFLOAT3 position;
	float range;

	// Direction, Spot ���·� �ϳ��� 4���� ���Ϳ� ä���ִ´�.
	XMFLOAT3 direction;
	float spot;

	// (A0,A1,A2,Pad) ���·� �ϳ��� 4���� ���Ϳ� ä�� �ִ´�.
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