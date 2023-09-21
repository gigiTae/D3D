#include "MathPCH.h"
#include "Geometry.h"

std::vector<DM::Vertex1> DM::Geometry::GetBoxVertices()
{
	XMFLOAT4 red = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	return {
		// 전면
		Vertex1(XMFLOAT3(-1.f,-1.f,-1.f),red),
		Vertex1(XMFLOAT3(-1.f,1.f,-1.f),red),
		Vertex1(XMFLOAT3(1.f,-1.f,-1.f),red),
		Vertex1(XMFLOAT3(1.f,1.f,-1.f),red),
		// 후면
		Vertex1(XMFLOAT3(-1.f,-1.f,1.f),red),
		Vertex1(XMFLOAT3(-1.f,1.f,1.f),red),
		Vertex1(XMFLOAT3(1.f,-1.f,1.f),red),
		Vertex1(XMFLOAT3(1.f,1.f,1.f),red) };

}

std::vector<unsigned int> DM::Geometry::GetBoxIndices()
{
	return {
		// 전면
		0, 1, 2,  // 삼각형 1
		1, 3, 2,  // 삼각형 2

		// 뒷면
		4, 5, 6,  // 삼각형 3
		5, 7, 6,  // 삼각형 4

		// 왼쪽 면
		0, 4, 2,  // 삼각형 5
		4, 6, 2,  // 삼각형 6

		// 오른쪽 면
		1, 5, 3,  // 삼각형 7
		5, 7, 3,  // 삼각형 8

		// 위쪽 면
		1, 0, 5,  // 삼각형 9
		0, 4, 5,  // 삼각형 10

		// 아래쪽 면
		2, 3, 6,  // 삼각형 11
		3, 7, 6   // 삼각형 12
	};
}

void DM::Geometry::GetCylinder(float bottomRadius, float topRadius
	, float height, unsigned int sliceCount, unsigned int stackCount
	, std::vector<Vertex1>& vertex, std::vector<unsigned int>& index)
{
	vertex.clear();
	index.clear();

	// 더미들을 만든다.
	float stackHegiht = height / stackCount;

	// 한 단계 위로 올라갈 때의 
	// 반지름 변화량을 구한다
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1; 

	// 최하단 고리에서 최상단 고리까지 훓으면서 
	// 각 고리의 점정들을 계산한다.
	for (unsigned int i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHegiht;
		float r = bottomRadius + i * radiusStep;

		// 현재 고리의 정점들
		float dTheta = 2.f * DM::PI / sliceCount;
		for (unsigned int j = 0; j <= sliceCount; ++j)
		{
			Vertex1 v{};
			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			v.position = DirectX::XMFLOAT3(r * c, y ,r * s);
			v.color = DirectX::XMFLOAT4(0.f, 1.f, 0.f, 1.f);

			vertex.push_back(std::move(v));
		}
	}

	// 원기둥에 텍스처를 제대로 입히려면 첫 정점과 마지막 정점이 중복되어야한다.
	// 이를 위해 정점개수를 조각 개수보다 하나 더 많게 한다.
	unsigned int ringVertexCount = sliceCount + 1;

	// 각 더미마다 색인들을 계산한다
	for (unsigned int i = 0; i < stackCount; ++i)
	{
		for (unsigned int j = 0; j < sliceCount; ++j)
		{
			index.push_back(i * ringVertexCount + j);
			index.push_back((i+1) * ringVertexCount + j);
			index.push_back((i+1) * ringVertexCount + j+1);

			index.push_back(i * ringVertexCount + j);
			index.push_back((i+1) * ringVertexCount + j+1);
			index.push_back(i * ringVertexCount + j+1);
		}
	}


}
