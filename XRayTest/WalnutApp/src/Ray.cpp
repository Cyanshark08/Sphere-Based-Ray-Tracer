#include "Ray.h"

using namespace XRay;

XRay::Ray::Ray()
	: m_Direction(),
	m_Origin()
{}

XRay::Ray::Ray(const Vec3& p_Direction, const Vec3& p_Origin)
	: m_Direction(p_Direction),
	m_Origin(p_Origin)
{}

const Vec3& Ray::getDirection() const
{
	return m_Direction;
}

const Vec3& Ray::getOrigin() const
{
	return m_Origin;
}

const Vec3& Ray::setDirection(const Vec3& p_NewDirection)
{
	m_Direction = p_NewDirection;
	return m_Direction;
}

const Vec3& Ray::setOrigin(const Vec3& p_NewOrigin)
{
	m_Origin = p_NewOrigin;
	return m_Origin;
}
