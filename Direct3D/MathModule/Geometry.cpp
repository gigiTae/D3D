#include "MathPCH.h"
#include "Geometry.h"

std::vector<DM::Vertex1> DM::Geometry::GetBoxVertices()
{
	XMFLOAT4 green = XMFLOAT4(0.f, 1.f, 0.f, 1.f);

	return {
		// ����
		Vertex1(XMFLOAT3(-1.f,-1.f,-1.f),green),
		Vertex1(XMFLOAT3(-1.f,1.f,-1.f),green),
		Vertex1(XMFLOAT3(1.f,-1.f,-1.f),green),
		Vertex1(XMFLOAT3(1.f,1.f,-1.f),green),
		// �ĸ�
		Vertex1(XMFLOAT3(-1.f,-1.f,1.f),green),
		Vertex1(XMFLOAT3(-1.f,1.f,1.f),green),
		Vertex1(XMFLOAT3(1.f,-1.f,1.f),green),
		Vertex1(XMFLOAT3(1.f,1.f,1.f),green) };

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
