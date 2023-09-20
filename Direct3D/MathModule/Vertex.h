#pragma once

namespace DM
{
	struct Vertex1
	{
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

