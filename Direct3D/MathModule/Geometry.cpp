#include "MathPCH.h"
#include "Geometry.h"

std::vector<DM::Vertex1> DM::Geometry::GetBoxVertices()
{
	XMFLOAT4 green = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	return {
		// Àü¸é
		Vertex1(XMFLOAT3(-1.f,-1.f,-1.f),green),
		Vertex1(XMFLOAT3(-1.f,1.f,-1.f),green),
		Vertex1(XMFLOAT3(1.f,-1.f,-1.f),green),
		Vertex1(XMFLOAT3(1.f,1.f,-1.f),green),
		// ÈÄ¸é
		Vertex1(XMFLOAT3(-1.f,-1.f,1.f),green),
		Vertex1(XMFLOAT3(-1.f,1.f,1.f),green),
		Vertex1(XMFLOAT3(1.f,-1.f,1.f),green),
		Vertex1(XMFLOAT3(1.f,1.f,1.f),green) };

}

std::vector<unsigned int> DM::Geometry::GetBoxIndices()
{
	return {
		// Àü¸é
		0, 1, 2,  // »ï°¢Çü 1
		1, 3, 2,  // »ï°¢Çü 2

		// µÞ¸é
		4, 5, 6,  // »ï°¢Çü 3
		5, 7, 6,  // »ï°¢Çü 4

		// ¿ÞÂÊ ¸é
		0, 4, 2,  // »ï°¢Çü 5
		4, 6, 2,  // »ï°¢Çü 6

		// ¿À¸¥ÂÊ ¸é
		1, 5, 3,  // »ï°¢Çü 7
		5, 7, 3,  // »ï°¢Çü 8

		// À§ÂÊ ¸é
		1, 0, 5,  // »ï°¢Çü 9
		0, 4, 5,  // »ï°¢Çü 10

		// ¾Æ·¡ÂÊ ¸é
		2, 3, 6,  // »ï°¢Çü 11
		3, 7, 6   // »ï°¢Çü 12
	};
}
