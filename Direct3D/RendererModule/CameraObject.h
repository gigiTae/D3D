#pragma once

/// <summary>
///  카메라 오브젝트 
/// 
/// 
/// </summary>
class CameraObject
{
public:
	CameraObject();
	~CameraObject();
	void Initialize(int width, int hegiht, XMVECTOR position);
	void Update();
	void Finalize();

	DirectX::XMVECTOR GetPosition() const { return m_position; }
public:
	DirectX::XMMATRIX GetViewMatrix() const { return m_viewMatrix; }
	DirectX::XMMATRIX GetProjectMatrix() const { return m_projectMatrix; }
	void SetFovAngleY(float val) { m_fovAngleY = val; }
	float GetFovAngleY() const { return m_fovAngleY; }
	void SetNearZ(float val) { m_nearZ = val; }
	float GetNearZ() const { return m_nearZ; }
	void SetFarZ(float val) { m_farZ = val; }
	float GetFarZ() const { return m_farZ; }

	void MoveCameraZ(float distace);
	void MoveCameraX(float distace);
	void MoveCameraY(float distace);
	void RotateCameraY(float angle);
	void RotateCameraX(float angle);

private:
	DirectX::XMMATRIX m_viewMatrix; // 카메라 변환 행렬 
	DirectX::XMMATRIX m_projectMatrix; // 투영 행렬

	XMVECTOR m_position;  // 카메라 위치 
	XMVECTOR m_rotation;  // 카메라 각도

	XMVECTOR m_cameraDirection; // 카메라가 바라보는 방향 벡터 z축
	XMVECTOR m_worldUp;


	float m_fovAngleY;   // 시야각 
	float m_aspectRatio;
	float m_nearZ;
	float m_farZ;

};

