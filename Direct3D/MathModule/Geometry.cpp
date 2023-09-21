#include "MathPCH.h"
#include "Geometry.h"

std::vector<DM::Vertex1> DM::Geometry::GetBoxVertices()
{
	XMFLOAT4 red = XMFLOAT4(1.f, 0.f, 0.f, 1.f);

	return {
		// ����
		Vertex1(XMFLOAT3(-1.f,-1.f,-1.f),red),
		Vertex1(XMFLOAT3(-1.f,1.f,-1.f),red),
		Vertex1(XMFLOAT3(1.f,-1.f,-1.f),red),
		Vertex1(XMFLOAT3(1.f,1.f,-1.f),red),
		// �ĸ�
		Vertex1(XMFLOAT3(-1.f,-1.f,1.f),red),
		Vertex1(XMFLOAT3(-1.f,1.f,1.f),red),
		Vertex1(XMFLOAT3(1.f,-1.f,1.f),red),
		Vertex1(XMFLOAT3(1.f,1.f,1.f),red) };

}

std::vector<unsigned int> DM::Geometry::GetBoxIndices()
{
	return {
		// ����
		0, 1, 2,  // �ﰢ�� 1
		1, 3, 2,  // �ﰢ�� 2

		// �޸�
		4, 5, 6,  // �ﰢ�� 3
		5, 7, 6,  // �ﰢ�� 4

		// ���� ��
		0, 4, 2,  // �ﰢ�� 5
		4, 6, 2,  // �ﰢ�� 6

		// ������ ��
		1, 5, 3,  // �ﰢ�� 7
		5, 7, 3,  // �ﰢ�� 8

		// ���� ��
		1, 0, 5,  // �ﰢ�� 9
		0, 4, 5,  // �ﰢ�� 10

		// �Ʒ��� ��
		2, 3, 6,  // �ﰢ�� 11
		3, 7, 6   // �ﰢ�� 12
	};
}

void DM::Geometry::GetCylinder(float bottomRadius, float topRadius
	, float height, unsigned int sliceCount, unsigned int stackCount
	, std::vector<Vertex1>& vertex, std::vector<unsigned int>& index)
{
	vertex.clear();
	index.clear();

	// ���̵��� �����.
	float stackHegiht = height / stackCount;

	// �� �ܰ� ���� �ö� ���� 
	// ������ ��ȭ���� ���Ѵ�
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	unsigned int ringCount = stackCount + 1; 

	// ���ϴ� ������ �ֻ�� ������ �f���鼭 
	// �� ���� �������� ����Ѵ�.
	for (unsigned int i = 0; i < ringCount; ++i)
	{
		float y = -0.5f * height + i * stackHegiht;
		float r = bottomRadius + i * radiusStep;

		// ���� ���� ������
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

	// ����տ� �ؽ�ó�� ����� �������� ù ������ ������ ������ �ߺ��Ǿ���Ѵ�.
	// �̸� ���� ���������� ���� �������� �ϳ� �� ���� �Ѵ�.
	unsigned int ringVertexCount = sliceCount + 1;

	// �� ���̸��� ���ε��� ����Ѵ�
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
