/*
   Class Name:

	  CCamera

   Description:

	  position the camera
*/

#pragma once

class CCamera
{
public:
	double eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz;

public:
	CCamera();
	~CCamera();

	void SetPosition(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz);

	void MoveForward(double a);
	void MoveBackward(double a);
	void RotateLeft(double a);
	void RotateRight(double a);
	void StrafeLeft(double a);
	void StrafeRight(double a);
};
