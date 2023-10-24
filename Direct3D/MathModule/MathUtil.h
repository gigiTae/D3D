#pragma once


namespace MathModule
{
	static constexpr float PI = { 3.14159265358979323846f };
	static constexpr float TwoPI = { 2.f * PI };
	static constexpr float HalfPI = { 1.57079632679f };
	static constexpr float InvPI = { 0.31830988618f };

	float AngleFromXY(float x, float y);


	XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

}