#pragma once

namespace GrapicsEngine
{
	/// <summary>
	/// ���� �⺻���� ����
	/// 
	/// 
	///  
	/// </summary>
	class BaseVertex
	{
	public:
		BaseVertex();
		~BaseVertex();

		XMFLOAT3 position; // ��ġ��
		XMFLOAT3 normal;   // �ؽ��� ��ǥ
		XMFLOAT2 uv;       // �븻
		XMFLOAT3 tangent;  // ź��Ʈ 
	};

	/// <summary>
	/// ��ġ, �÷��� �����ϴ� ����
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