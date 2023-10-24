#pragma once

namespace RendererModule
{
	/// <summary>
	/// 가장 기본적인 정점 
	/// </summary>
	class BaseVertex
	{
	public:
		BaseVertex();
		BaseVertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
			: position(p), normal(n), tangent(t), uv(uv) {}
		BaseVertex(
			float px, float py, float pz,
			float nx, float ny, float nz,
			float tx, float ty, float tz,
			float u, float v)
			: position(px, py, pz), normal(nx, ny, nz),
			tangent(tx, ty, tz), uv(u, v) {}

		~BaseVertex();

		XMFLOAT3 position; // 위치값
		XMFLOAT3 normal;   // 텍스터 좌표
		XMFLOAT3 tangent;  // 탄젠트 
		DirectX::XMFLOAT2 uv;       // 노말
	};

	/// <summary>
	/// 위치, 컬러만 존재하는 정점
	/// </summary>
	class SimpleVertex
	{
	public:
		SimpleVertex();
		~SimpleVertex();

		XMFLOAT3 position;
		XMFLOAT4 color;

	};



}