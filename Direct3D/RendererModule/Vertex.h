#pragma once

namespace GrapicsEngine
{
	/// <summary>
	/// 가장 기본적인 정점
	/// 
	/// 
	///  
	/// </summary>
	class BaseVertex
	{
	public:
		BaseVertex();
		~BaseVertex();

		XMFLOAT3 position; // 위치값
		XMFLOAT3 normal;   // 텍스터 좌표
		XMFLOAT2 uv;       // 노말
		XMFLOAT3 tangent;  // 탄젠트 
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