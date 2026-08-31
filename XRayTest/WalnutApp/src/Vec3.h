#pragma once
#include <stdint.h>

namespace XRay
{
	struct Vec3
	{
	public:
		//Constructors
		Vec3() : x(0.f), y(0.f), z(0.f) {} //Default

		Vec3(float p_X, float p_Y, float p_Z) : x(p_X), y(p_Y), z(p_Z) {} //Constructor given 3 params

		Vec3(float p_Num) : x(p_Num), y(p_Num), z(p_Num) {} //Constructor given 1 param

		Vec3(const Vec3& other) : x(other.x), y(other.y), z(other.z) {} //Copy Constructor
		//Access Useful Quantities
		float magnitude() const;

		//static
		static float dot(const Vec3& p_Left, const Vec3& p_Right); //dot product
		static Vec3& normalize(Vec3& p_Vec);
		static Vec3 reflect(const Vec3& p_Incident, const Vec3& p_Normal);

		Vec3& clamp(float p_LeftBound, float p_RightBound);

		//Utility
		uint32_t toABGR() const;

		//overloaded operators
		Vec3 operator+(const Vec3& other) const;
		Vec3 operator-(const Vec3& other) const;

		Vec3 operator-() const;

		Vec3 operator*(float p_Scalar) const;

		Vec3& operator+=(const Vec3& other);
		Vec3& operator-=(const Vec3& other);

		Vec3& operator*=(float p_Scalar);
		Vec3& operator/=(float p_Scalar);

		void printVector() const;

	public:
		float x, y, z;
	};

}
