#include "RendererPCH.h"
#include "CameraObject.h"

CameraObject::CameraObject()
	:m_projectMatrix()
	,m_worldUp()
	,m_cameraDirection()
	,m_position()
	,m_fovAngleY(0.f)
	,m_aspectRatio(0.f)
	, m_nearZ(0.f)
	, m_viewMatrix()
	,m_farZ(0.f)
{
}

CameraObject::~CameraObject()
{
	
}

void CameraObject::Initialize(int width, int hegiht, XMVECTOR position)
{
	// 카메라 초기화 
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(hegiht);
	m_nearZ = 1.f;
	m_farZ = 1000.f; 
	m_fovAngleY = DM::HalfPI; 

	// 카메라 위치는 원점 
	SetPosition(position);
	XMVECTOR zero = XMVectorZero();
	m_worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	// 카메라는 원점을 바라본다 
	m_cameraDirection = zero - GetPosition();
	
	// 행렬 계산
	m_viewMatrix = XMMatrixLookToLH(GetPosition(), m_cameraDirection, m_worldUp);
	m_projectMatrix = XMMatrixPerspectiveFovLH(m_fovAngleY, m_aspectRatio, m_nearZ, m_farZ);

}

void CameraObject::Update()
{
	m_viewMatrix = XMMatrixLookToLH(GetPosition(), m_cameraDirection, m_worldUp);
	m_projectMatrix = XMMatrixPerspectiveFovLH(m_fovAngleY, m_aspectRatio, m_nearZ, m_farZ);
}

void CameraObject::Finalize()
{

}

void CameraObject::MoveCameraZ(float distace)
{
	XMVECTOR normal = XMVector3Normalize(m_cameraDirection);
	
	SetPosition(GetPosition() + normal * distace);

	Update();
}

void CameraObject::MoveCameraX(float distace)
{
	XMVECTOR normal = XMVector3Normalize(m_cameraDirection);

	XMVECTOR xAxis = XMVector3Cross(m_worldUp, normal);

	SetPosition(GetPosition() + xAxis * distace);
	
	Update();
}

void CameraObject::MoveCameraY(float distace)
{
	XMVECTOR normal = XMVector3Normalize(m_cameraDirection);

	XMVECTOR xAxis = XMVector3Cross(m_worldUp, normal);

	XMVECTOR yAxis = XMVector3Cross(normal, xAxis);

	SetPosition(GetPosition() + yAxis * distace);

	Update();
}

void CameraObject::RotateCameraY(float angle)
{

	XMVECTOR zAxis = XMVector3Normalize(m_cameraDirection);

	XMVECTOR xAxis = XMVector3Cross(m_worldUp, zAxis);

	XMVECTOR yAxis = XMVector3Cross(zAxis, xAxis);

	XMMATRIX rotainoMat = XMMatrixRotationAxis(yAxis, angle);

	m_cameraDirection = XMVector4Transform(m_cameraDirection, rotainoMat);;

	Update();
}

void CameraObject::RotateCameraX(float angle)
{

	XMVECTOR zAxis = XMVector3Normalize(m_cameraDirection);

	XMVECTOR xAxis = XMVector3Cross(m_worldUp, zAxis);

	XMVECTOR yAxis = XMVector3Cross(zAxis, xAxis);

	XMMATRIX rotainoMat = XMMatrixRotationAxis(xAxis, angle);


	m_cameraDirection = XMVector4Transform(m_cameraDirection, rotainoMat);

	Update();
}
