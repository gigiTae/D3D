#include "RendererPCH.h"
#include "Camera.h"

RendererModule::Camera::Camera()
	: m_position(0.0f, 0.0f, 0.0f),
	m_right(1.0f, 0.0f, 0.0f),
	m_up(0.0f, 1.0f, 0.0f),
	m_look(0.0f, 0.0f, 1.0f)
{
	SetLens(0.25f * MathModule::PI, 1.0f, 0.0001f, 1000.0f);
}

RendererModule::Camera::~Camera()
{

}

XMVECTOR RendererModule::Camera::GetPositionXM() const
{
	return XMLoadFloat3(&m_position);
}

XMFLOAT3 RendererModule::Camera::GetPosition() const
{
	return m_position;
}


void RendererModule::Camera::SetPosition(float x, float y, float z)
{
	m_position = XMFLOAT3(x, y, z);
}

void RendererModule::Camera::SetPosition(const XMFLOAT3& v)
{
	m_position = v;
}

XMVECTOR RendererModule::Camera::GetRightXM() const
{
	return XMLoadFloat3(&m_right);
}

XMFLOAT3 RendererModule::Camera::GetRight() const
{
	return m_right;
}

XMVECTOR RendererModule::Camera::GetUpXM() const
{
	return XMLoadFloat3(&m_up);
}

XMFLOAT3 RendererModule::Camera::GetUp() const
{
	return m_up;
}

XMVECTOR RendererModule::Camera::GetLookXM() const
{
	return XMLoadFloat3(&m_look);
}

XMFLOAT3 RendererModule::Camera::GetLook() const
{
	return m_look;
}

float RendererModule::Camera::GetNearZ() const
{
	return m_nearZ;
}

float RendererModule::Camera::GetFarZ() const
{
	return m_farZ;
}

float RendererModule::Camera::GetAspect() const
{
	return m_aspect;
}

float RendererModule::Camera::GetFovY() const
{
	return m_fovY;
}

float RendererModule::Camera::GetFovX() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / m_nearZ);
}

float RendererModule::Camera::GetNearWindowWidth() const
{
	return m_aspect * m_nearWindowHeight;
}

float RendererModule::Camera::GetNearWindowHeight() const
{
	return m_nearWindowHeight;
}

float RendererModule::Camera::GetFarWindowWidth() const
{
	return m_aspect * m_farWindowHeight;
}

float RendererModule::Camera::GetFarWindowHeight() const
{
	return m_farWindowHeight;
}

void RendererModule::Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	m_fovY = fovY;
	m_aspect = aspect;
	m_nearZ = zn;
	m_farZ = zf;

	m_nearWindowHeight = 2.0f * m_nearZ * tanf(0.5f * m_fovY);
	m_farWindowHeight = 2.0f * m_farZ * tanf(0.5f * m_fovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_fovY, m_aspect, m_nearZ, m_farZ);
	XMStoreFloat4x4(&m_proj, P);
}

void RendererModule::Camera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	// 시선벡터와 월드업을 외적하면 right벡터가 나오고
	// 다시 그 right벡터와 시선벡터를 외적하면 내 up벡터가 나옴
	XMVECTOR L = XMVector3Normalize(XMVectorSubtract(target, pos));
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(worldUp, L));
	XMVECTOR U = XMVector3Cross(L, R);

	XMStoreFloat3(&m_position, pos);
	XMStoreFloat3(&m_look, L);
	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
}

void RendererModule::Camera::LookAt(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& up)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&up);

	LookAt(P, T, U);
}

XMMATRIX RendererModule::Camera::View() const
{
	return XMLoadFloat4x4(&m_view);
}

XMMATRIX RendererModule::Camera::Proj() const
{
	return XMLoadFloat4x4(&m_proj);
}

XMMATRIX RendererModule::Camera::ViewProj() const
{
	return XMMatrixMultiply(View(), Proj());
}

void RendererModule::Camera::Strafe(float d)
{
	// mPosition += d*mRight
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_right);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, r, p));
}

void RendererModule::Camera::Walk(float d)
{
	// mPosition += d*mLook
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_look);
	XMVECTOR p = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(s, l, p));
}

void RendererModule::Camera::WorldUpDown(float d)
{
	// mPosition += d * mUp
	XMVECTOR scale = XMVectorReplicate(d);
	XMVECTOR up = XMLoadFloat3(&m_up);
	XMVECTOR position = XMLoadFloat3(&m_position);
	XMStoreFloat3(&m_position, XMVectorMultiplyAdd(scale, up, position));
}

void RendererModule::Camera::Pitch(float angle)
{
	// Rotate up and look vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_right), angle);

	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
}

void RendererModule::Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_right, XMVector3TransformNormal(XMLoadFloat3(&m_right), R));
	XMStoreFloat3(&m_up, XMVector3TransformNormal(XMLoadFloat3(&m_up), R));
	XMStoreFloat3(&m_look, XMVector3TransformNormal(XMLoadFloat3(&m_look), R));
}

void RendererModule::Camera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_right);
	XMVECTOR U = XMLoadFloat3(&m_up);
	XMVECTOR L = XMLoadFloat3(&m_look);
	XMVECTOR P = XMLoadFloat3(&m_position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_right, R);
	XMStoreFloat3(&m_up, U);
	XMStoreFloat3(&m_look, L);

	m_view(0, 0) = m_right.x;
	m_view(1, 0) = m_right.y;
	m_view(2, 0) = m_right.z;
	m_view(3, 0) = x;

	m_view(0, 1) = m_up.x;
	m_view(1, 1) = m_up.y;
	m_view(2, 1) = m_up.z;
	m_view(3, 1) = y;

	m_view(0, 2) = m_look.x;
	m_view(1, 2) = m_look.y;
	m_view(2, 2) = m_look.z;
	m_view(3, 2) = z;

	m_view(0, 3) = 0.0f;
	m_view(1, 3) = 0.0f;
	m_view(2, 3) = 0.0f;
	m_view(3, 3) = 1.0f;
}
