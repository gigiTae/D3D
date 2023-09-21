#pragma once

namespace DM
{
	struct Vertex1
	{
		Vertex1():position(),color(){}
		Vertex1(XMFLOAT3 position, XMFLOAT4 color)
			:position(position),color(color)
		{}

		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;	
	};

	struct Vertex2
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 tex0;
		DirectX::XMFLOAT2 tex1;
	};

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;

	};
}

