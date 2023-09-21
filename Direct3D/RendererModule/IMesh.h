#pragma once

/// <summary>
/// Mesh 인터페이스
/// </summary>
class IMesh abstract
{
public:
	IMesh() {}
	virtual ~IMesh() {}

public:
	virtual void Initialize() abstract;
	virtual void Render() abstract;
	virtual void Update(const XMMATRIX& world, const XMMATRIX& view, const XMMATRIX& proj) abstract;
};

