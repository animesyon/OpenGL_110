/*
   Class Name:

	  CCamera

   Description:

	  position the camera
*/

#include "framework.h"
#include "camera.h"

// constructor
CCamera::CCamera()
{
	eyex = 1.0;      eyey = 1.0;      eyez = 1.0;
	centerx = 0.0;      centery = 0.0;      centerz = 0.0;
	upx = 0.0;      upy = 1.0;      upz = 0.0;
}

// destructor
CCamera::~CCamera()
{
}

//
void CCamera::SetPosition(double ex, double ey, double ez, double cx, double cy, double cz, double ux, double uy, double uz)
{
	eyex = ex;      eyey = ey;      eyez = ez;
	centerx = cx;      centery = cy;      centerz = cz;
	upx = ux;      upy = uy;      upz = uz;
}

//
void CCamera::MoveForward(double a)
{
	double s, sx, sy, sz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	// add vector s to eye and center vector
	eyex += (sx * a);
	eyey += (sy * a);
	eyez += (sz * a);

	centerx += (sx * a);
	centery += (sy * a);
	centerz += (sz * a);
}

//
void CCamera::MoveBackward(double a)
{
	double s, sx, sy, sz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	// subtract vector s from eye and center vector
	eyex -= (sx * a);
	eyey -= (sy * a);
	eyez -= (sz * a);

	centerx -= (sx * a);
	centery -= (sy * a);
	centerz -= (sz * a);
}

//
void CCamera::RotateLeft(double a)
{
	double s, sx, sy, sz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	double m11, m12, m13, m21, m22, m23, m31, m32, m33;
	double x, y, z, w;
	double rx, ry, rz;

	// create quarternion matrix
	double angle = a / 180.0f * M_PI;
	double sine = sin(angle / 2.0f);

	x = upx * sine;
	y = upy * sine;
	z = upz * sine;
	w = cos(angle / 2.0f);

	m11 = 1.0f - 2.0f * (y * y + z * z);
	m21 = 2.0f * (x * y + z * w);
	m31 = 2.0f * (x * z - y * w);

	m12 = 2.0f * (x * y - z * w);
	m22 = 1.0f - 2.0f * (x * x + z * z);
	m32 = 2.0f * (z * y + x * w);

	m13 = 2.0f * (x * z + y * w);
	m23 = 2.0f * (y * z - x * w);
	m33 = 1.0f - 2.0f * (x * x + y * y);

	// multiply quarternion matrix by vector s
	// to get rotated vector
	rx = m11 * sx + m12 * sy + m13 * sz;
	ry = m21 * sx + m22 * sy + m23 * sz;
	rz = m31 * sx + m32 * sy + m33 * sz;

	// add vector r to eye vector
	// to rotate the camera
	centerx = eyex + rx;
	centery = eyey + ry;
	centerz = eyez + rz;

}

//
void CCamera::RotateRight(double a)
{
	double s, sx, sy, sz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	double m11, m12, m13, m21, m22, m23, m31, m32, m33;
	double x, y, z, w;
	double rx, ry, rz;

	// create quarternion matrix
	double angle = -a / 180.0f * M_PI;
	double sine = sin(angle / 2.0f);

	x = upx * sine;
	y = upy * sine;
	z = upz * sine;
	w = cos(angle / 2.0f);

	m11 = 1.0f - 2.0f * (y * y + z * z);
	m21 = 2.0f * (x * y + z * w);
	m31 = 2.0f * (x * z - y * w);

	m12 = 2.0f * (x * y - z * w);
	m22 = 1.0f - 2.0f * (x * x + z * z);
	m32 = 2.0f * (z * y + x * w);

	m13 = 2.0f * (x * z + y * w);
	m23 = 2.0f * (y * z - x * w);
	m33 = 1.0f - 2.0f * (x * x + y * y);

	// multiply quarternion matrix by vector s
	// to get rotated vector
	rx = m11 * sx + m12 * sy + m13 * sz;
	ry = m21 * sx + m22 * sy + m23 * sz;
	rz = m31 * sx + m32 * sy + m33 * sz;

	// add vector r to eye vector
	// to rotate the camera
	centerx = eyex + rx;
	centery = eyey + ry;
	centerz = eyez + rz;
}

//
void CCamera::StrafeLeft(double a)
{
	double s, sx, sy, sz, p, px, py, pz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	// get cross product of vector s and up vector
	// to get the vector that is perpendicular to both vector s and up vector
	px = (sy * upz) - (sz * upy);
	py = (sz * upx) - (sx * upz);
	pz = (sx * upy) - (sy * upx);

	// normalize vector
	p = sqrt(px * px + py * py + pz * pz);
	px /= p;
	py /= p;
	pz /= p;

	// subtract vector p from eye and center vector
	eyex -= (px * a);
	eyey -= (py * a);
	eyez -= (pz * a);

	centerx -= (px * a);
	centery -= (py * a);
	centerz -= (pz * a);

}

//
void CCamera::StrafeRight(double a)
{
	double s, sx, sy, sz, p, px, py, pz;

	// get line of sight vector
	sx = centerx - eyex;
	sy = centery - eyey;
	sz = centerz - eyez;

	// normalize vector
	s = sqrt(sx * sx + sy * sy + sz * sz);
	sx /= s;
	sy /= s;
	sz /= s;

	// get cross product of vector s and up vector
	// to get the vector that is perpendicular to both vector s and up vector
	px = (sy * upz) - (sz * upy);
	py = (sz * upx) - (sx * upz);
	pz = (sx * upy) - (sy * upx);

	// normalize vector
	p = sqrt(px * px + py * py + pz * pz);
	px /= p;
	py /= p;
	pz /= p;

	// add vector p from eye and center vector
	eyex += (px * a);
	eyey += (py * a);
	eyez += (pz * a);

	centerx += (px * a);
	centery += (py * a);
	centerz += (pz * a);

}

//
