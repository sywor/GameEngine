#pragma once
#include "stdafx.h"

#define TWO_PI		6.283185307179586476925286766559
#define DEG_TO_RAD	0.01745329251994329576923690768489

class ICamera
{
private:
	ICamera(const ICamera&) = delete;

protected:
	ICamera()			{}
	virtual ~ICamera()	{}

public:
	virtual bool init(unsigned int clientWidth, unsigned int clientHeight)	= 0;

	virtual void		update()						= 0;
	virtual MATRIX4X4	getViewMatrix()			const	= 0;
	virtual MATRIX4X4	getProjectionMatrix()	const	= 0;

	virtual void	setMovementToggle(int i, int v)				{}
	virtual void	adjustHeadingPitch(float hRad, float pRad)	{}
	virtual void	setPositionAndView(float x, float y, float z, float hDeg, float pDeg) = 0;
};