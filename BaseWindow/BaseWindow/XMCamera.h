#pragma once
#include <Windows.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <d3d.h>
#include <d3d11.h>
#include "ICamera.h"
#include "hrTimer.h"

using namespace DirectX;

class XMCamera
	: public ICamera
{
private:
	XMFLOAT3	m_position;
	XMFLOAT3	m_lookAt;
	XMFLOAT3	m_up;
	XMFLOAT3	m_forward;
	XMFLOAT3	m_right;

	XMFLOAT4X4	m_viewMatrix;
	XMFLOAT4X4	m_projectionMatrix;
	XMFLOAT4X4	m_rotationMatrix;

	float m_roll;
	float m_pitch;
	float m_yaw;

	int movementToggles[4];	//forward:back:left:right
	float movementSpeed;

	timer m_camTimer;

protected:
public:

private:
	void updateView();
	void setPerspectiveProjectionLH(float fov, float width, float height, float zNear, float zFar);

protected:
public:
	XMCamera();
	~XMCamera();

	void update() final;

	bool init(unsigned int clientWidth, unsigned int clientHeight)				final;
	void setPositionAndView(float x, float y, float z, float hDeg, float pDeg)	final;
	void transposeMatrix(XMFLOAT4X4& _mat4x4);

	void setMovementToggle(int i, int v)			final;
	void adjustHeadingPitch(float hRad, float pRad) final;

	MATRIX4X4	getViewMatrix()			const final { return m_viewMatrix; }
	MATRIX4X4	getProjectionMatrix()	const final { return m_projectionMatrix; }
	//MATRIX4X4	getViewMatrix()			const final { return MATRIX4X4(m_viewMatrix); }
	//MATRIX4X4	getProjectionMatrix()	const final { return MATRIX4X4(m_projectionMatrix); }
};