#include "Sphere.h"
#include <cmath>
#include <limits>
#include "glm/common.hpp"

using namespace XRay;

Sphere::Sphere() :
	m_Center(0.f, 0.f, 0.f),
	m_Radius(1.f),
	m_Color(0.f, 1.f, 1.f),
	Entity()
{
}

const Vec3& Sphere::getCenter() const
{
	return m_Center;
}

//Format: BGR
const Vec3& Sphere::getColor() const
{
	return m_Color;
}

const float Sphere::getRadius() const
{
	return m_Radius;
}

Vec3* Sphere::getMutableCenter()
{
	return &m_Center;
}

Vec3* Sphere::getMutableColor()
{
	return &m_Color;
}

float* Sphere::getMutableRadius()
{
	return &m_Radius;
}

const Vec3& Sphere::moveBy(const Vec3& p_Increment)
{
	m_Center += p_Increment;
	return m_Center;
}

const Vec3& Sphere::moveTo(const Vec3& p_NewPosition)
{
	m_Center = p_NewPosition;
	return m_Center;
}

float Sphere::getIntersectionDistance(const Ray& p_Ray)
{
	Vec3 rayDirection = p_Ray.getDirection();
	Vec3 rayOrigin = p_Ray.getOrigin();

	float a = Vec3::dot(rayDirection, rayDirection);
	float b = 2 * (Vec3::dot(rayDirection, rayOrigin) - Vec3::dot(rayDirection, m_Center));
	float c = Vec3::dot(rayOrigin, rayOrigin) + Vec3::dot(m_Center, m_Center) - 2 * Vec3::dot(rayOrigin, m_Center) - (m_Radius * m_Radius);

	float discriminant = (b * b) - (4.f * a * c);

	if (discriminant < 0)
		return std::numeric_limits<float>::max();

	return ((-b - std::sqrt(discriminant)) / (2.f * a));
}

Vec3 Sphere::getIntersectionColor(const Vec3& p_LightDirection, const Vec3& p_Norm, const Vec3& p_BackgroundColor, const Ray& p_Ray)
{
	Vec3 lightDirection = p_LightDirection;
	Vec3::normalize(lightDirection);

	float lightScalar = glm::clamp(Vec3::dot(p_Norm, -lightDirection), 0.f, 1.f); //std::fmax(Vec3::dot(norm, -lightDirection), 0.f);

	if (lightScalar > 1)
		printf("\nLight Scaler: %.2f", lightScalar);


	Vec3 color = m_Color;
	color *= lightScalar;

	//ABGR
	return color;
}


