#include "Pendulum.h"
#include "glm/trigonometric.hpp"

void XRay::Pendulum::Update(float p_Delta)
{
	UpdateAngularAcceleration(p_Delta);
	UpdateAngularVelocity(p_Delta);
	UpdateAngle(p_Delta);
	UpdateBody();
}

void XRay::Pendulum::UpdateAngularAcceleration(float p_Delta)
{
	m_AngularAcceleration = -9.81 / m_StringLength * (float)glm::sin(m_CurrentAngle);
}

void XRay::Pendulum::UpdateAngularVelocity(float p_Delta)
{
	m_AngularVelocity += m_AngularAcceleration * p_Delta;
}

void XRay::Pendulum::UpdateAngle(float p_Delta)
{
	m_CurrentAngle += m_AngularVelocity * p_Delta;
}

void XRay::Pendulum::UpdateBody()
{
	m_Object.moveTo(Vec3(m_StringLength * glm::cos(m_CurrentAngle), 0.f, m_StringLength * glm::sin(m_CurrentAngle)) + m_Pivot.getCenter());
}

const XRay::Vec3& XRay::Pendulum::GetPivot() const
{
	return m_Pivot.getCenter();
}

const XRay::Sphere& XRay::Pendulum::getObject() const
{
	return m_Object;
}
