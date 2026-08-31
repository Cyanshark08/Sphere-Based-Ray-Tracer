#include "Vec3.h"
#include <cmath>
#include <stdio.h>

using namespace XRay;

float Vec3::dot(const Vec3& p_Left, const Vec3& p_Right)
{
    return p_Left.x * p_Right.x + p_Left.y * p_Right.y + p_Left.z * p_Right.z;
}

//dot product

Vec3& Vec3::normalize(Vec3& p_Vec)
{
	p_Vec /= p_Vec.magnitude();
	return p_Vec;
}

Vec3 XRay::Vec3::reflect(const Vec3& p_Incident, const Vec3& p_Normal)
{
    return p_Incident - p_Normal * 2.0 * dot(p_Normal, p_Incident);
}

Vec3& XRay::Vec3::clamp(float p_LeftBound, float p_RightBound)
{
    if (x < p_LeftBound)
        x = p_LeftBound;

    if (y < p_LeftBound)
        y= p_LeftBound;

    if (z < p_LeftBound)
        z = p_LeftBound;

    if (x > p_RightBound)
        x = p_RightBound;

    if (y > p_RightBound)
        y = p_RightBound;

    if (z > p_RightBound)
        z = p_RightBound;

    return *this;
}

uint32_t Vec3::toABGR() const
{
    uint8_t red = (uint8_t)(x * 255.f);
    uint8_t green = (uint8_t)(y * 255.f);
    uint8_t blue = (uint8_t)(z * 255.f);

    return ((0xff << 24) | (blue << 16) | (green << 8) | red);
}

float Vec3::magnitude() const
{
    return sqrt(x*x + y*y + z*z);
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(x + other.x, y + other.y, z + other.z);
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(x - other.x, y - other.y, z - other.z);
}

Vec3 Vec3::operator-() const
{
    return (*this * -1);
}

Vec3 Vec3::operator*(float p_Scalar) const
{
    return Vec3(x * p_Scalar, y * p_Scalar, z * p_Scalar);
}

Vec3& Vec3::operator+=(const Vec3& other)
{
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

Vec3& Vec3::operator-=(const Vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vec3& Vec3::operator*=(float p_Scalar)
{
    x *= p_Scalar;
    y *= p_Scalar;
    z *= p_Scalar;
    return *this;
}

Vec3& Vec3::operator/=(float p_Scalar)
{
	x /= p_Scalar;
	y /= p_Scalar;
	z /= p_Scalar;
	return *this;
}

void XRay::Vec3::printVector() const
{
    printf("%.2f, %.2f, %.2f", x, y, z);
}
