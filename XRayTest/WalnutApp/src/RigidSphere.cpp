#include "RigidSphere.h"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"
#include <cmath>

#define COLLISION_SPACING_ERROR 0.0001f

XRay::RigidSphere::RigidSphere()
	: Sphere()
{}

bool XRay::RigidSphere::WillBeOutOfBounds(const Vec3& p_AmountToBeMoved, const Vec3& p_PlaneBoundaries)
{
	Vec3 newCenter = this->getCenter() + p_AmountToBeMoved;
	if (newCenter.y - this->getRadius() - p_PlaneBoundaries.y <= COLLISION_SPACING_ERROR)
		return true;
	if (newCenter.x + this->getRadius() > p_PlaneBoundaries.x || newCenter.x - this->getRadius() < -p_PlaneBoundaries.x)
		return true;
	if (newCenter.z + this->getRadius() > p_PlaneBoundaries.z || newCenter.z - this->getRadius() < -p_PlaneBoundaries.z)
		return true;
	return false;
}

bool XRay::RigidSphere::CheckYFloorCollision(float p_YFloorPlane) const
{
	if (!this)
		return false;

	this->getCenter().y;
	this->getRadius();

	return (this->getCenter().y - this->getRadius() - p_YFloorPlane <= COLLISION_SPACING_ERROR) ? true : false;
}

void XRay::RigidSphere::CollideBodyWithFloor(float p_YFloorPlane)
{
	this->moveTo(Vec3(this->getCenter().x, p_YFloorPlane + this->getRadius() + COLLISION_SPACING_ERROR, this->getCenter().z));


	Vec3 momentumTransfered = Vec3(0.f, 1.f, 0.f) * Vec3::dot(this->GetMomentum(), Vec3(0.f, 1.f, 0.f));
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
}

bool XRay::RigidSphere::CheckXWallCollision(float p_XWallPlane) const
{
	return (fabs(this->getCenter().x - this->getRadius()) < p_XWallPlane) ? false : true;
}

void XRay::RigidSphere::CollideBodyWithXWall(float p_XWallPlane)
{
	this->moveTo(Vec3((this->getCenter().x - this->getRadius() > p_XWallPlane ? p_XWallPlane - COLLISION_SPACING_ERROR : -p_XWallPlane + COLLISION_SPACING_ERROR) + this->getRadius(), this->getCenter().y, this->getCenter().z));
	Vec3 momentumTransfered = Vec3(1.f, 0.f, 0.f) * Vec3::dot(this->GetMomentum(), Vec3(1.f, 0.f, 0.f));
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
}

bool XRay::RigidSphere::CheckZWallCollision(float p_ZWallPlane) const
{
	return (fabs(this->getCenter().z - this->getRadius()) < p_ZWallPlane) ? false : true;
}

void XRay::RigidSphere::CollideBodyWithZWall(float p_ZWallPlane)
{
	this->moveTo(Vec3(this->getCenter().x, this->getCenter().y, (this->getCenter().z - this->getRadius() > p_ZWallPlane ? p_ZWallPlane - COLLISION_SPACING_ERROR : -p_ZWallPlane + COLLISION_SPACING_ERROR) + this->getRadius()));
	Vec3 momentumTransfered = Vec3(0.f, 0.f, 1.f) * Vec3::dot(this->GetMomentum(), Vec3(0.f, 0.f, 1.f));
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
}

bool XRay::RigidSphere::CheckOtherRigidBodyCollision(RigidSphere& p_OtherBody)
{

	Vec3 dir = (this->getCenter() - p_OtherBody.getCenter());
	return (dir.magnitude() - (this->getRadius() + p_OtherBody.getRadius()) <= COLLISION_SPACING_ERROR) ? true : false;
}

void XRay::RigidSphere::s_CollideAllRigidBodies(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries)
{
	for (size_t i = 0; i < p_Bodies.size(); i++)
	{
		for (size_t j = i + 1; j < p_Bodies.size(); j++)
		{
			if (p_Bodies[i].CheckOtherRigidBodyCollision(p_Bodies[j]))
				s_Collide(p_Bodies[i], p_Bodies[j], p_PlaneBoundaries);
		}
	}
}

void XRay::RigidSphere::s_CheckCollision(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries)
{
	s_CollideAllRigidBodies(p_Bodies, p_PlaneBoundaries);

	size_t numOfBodies = p_Bodies.size();

	for (size_t i = 0; i < numOfBodies; i++)
	{
		if (p_Bodies[i].CheckYFloorCollision(p_PlaneBoundaries.y))
		{
			p_Bodies[i].CollideBodyWithFloor(p_PlaneBoundaries.y);
			p_Bodies[i].m_Acceleration = Vec3(0.f, 0.f, 0.f);
		}
		else
			p_Bodies[i].m_Acceleration = Vec3(0.f,-9.81f, 0.f);

		if (p_Bodies[i].CheckXWallCollision(p_PlaneBoundaries.x))
			p_Bodies[i].CollideBodyWithXWall(p_PlaneBoundaries.x);

		if (p_Bodies[i].CheckZWallCollision(p_PlaneBoundaries.z))
			p_Bodies[i].CollideBodyWithZWall(p_PlaneBoundaries.z);
	}
}

XRay::Vec3 XRay::RigidSphere::GetMomentum() const
{
	return this->m_Velocity * this->m_Mass;
}

float XRay::RigidSphere::GetKineticEnergy() const
{
	return 0.5f * m_Mass * m_Velocity.squareMagnitude();
}

float XRay::RigidSphere::GetPotentialEnergy(float p_FloorHeight) const
{
	return m_Mass * 9.81 * (this->getCenter().y - p_FloorHeight);
}

float XRay::RigidSphere::GetTotalEnergy(float p_FloorHeight) const
{
	return GetKineticEnergy() + GetPotentialEnergy(p_FloorHeight);
}

void XRay::RigidSphere::AddMomentum(Vec3 p_MomentumTransfered)
{
	p_MomentumTransfered /= this->m_Mass;
	this->m_Velocity += p_MomentumTransfered;
}

void XRay::RigidSphere::SetMomentum(Vec3 p_Momentum)
{
	p_Momentum /= this->m_Mass;
	this->m_Velocity = p_Momentum;
}

void XRay::RigidSphere::UpdatePosition(float p_DeltaTime)
{
	this->moveBy(m_Velocity * p_DeltaTime);
}

void XRay::RigidSphere::UpdateVelocity(float p_DeltaTime)
{
	this->m_Velocity += m_Acceleration * p_DeltaTime;
}

float* XRay::RigidSphere::GetMutableElasticity()
{
	return &this->m_Elasticity;
}

void XRay::RigidSphere::s_Collide(RigidSphere& p_Body1, RigidSphere& p_Body2, const Vec3& p_PlaneBoundaries)
{
	//dir1 points from body 1 to body 2
	Vec3 dir1 = (p_Body2.getCenter() - p_Body1.getCenter());

	Vec3 dir1Norm = dir1;
	Vec3::normalize(dir1Norm);

	//dir2 points from body 2 to body 1
	Vec3 dir2Norm = -dir1Norm;

	float centerDistance = dir1.magnitude();

	float sphereDistance = centerDistance - p_Body2.getRadius() - p_Body1.getRadius();

	Vec3 body1Displacement = dir2Norm * (COLLISION_SPACING_ERROR + abs(sphereDistance) / 2.f);
	Vec3 body2Displacement = dir1Norm * (COLLISION_SPACING_ERROR + abs(sphereDistance) / 2.f);
	if (p_Body1.WillBeOutOfBounds(body1Displacement, p_PlaneBoundaries))
		p_Body2.moveBy(body2Displacement * 2.f);
	else if (p_Body2.WillBeOutOfBounds(body2Displacement, p_PlaneBoundaries))
		p_Body1.moveBy(body1Displacement * 2.f);
	else
	{
		//printf("\n\tRegular displacement occurred");
		p_Body1.moveBy(body1Displacement);
		p_Body2.moveBy(body2Displacement);
	}

	const auto& n = dir1Norm;
	float elasticityFactor = p_Body1.m_Elasticity * p_Body2.m_Elasticity;

	float closingSpeed = Vec3::dot(p_Body1.m_Velocity, dir1Norm) + Vec3::dot(p_Body2.m_Velocity, dir2Norm);
	if (closingSpeed <= 0.f)
		return;

	float impulseMagnitude = (1 + elasticityFactor) * p_Body1.m_Mass * p_Body2.m_Mass / (p_Body1.m_Mass + p_Body2.m_Mass)
		* closingSpeed;

	Vec3 Impulse1 = dir2Norm * impulseMagnitude;

	Vec3 Impulse2 = -Impulse1;

	p_Body1.AddMomentum(Impulse1);
	p_Body2.AddMomentum(Impulse2);
}

void XRay::RigidSphere::s_UpdatePhysics(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries, float p_DeltaTime)
{
	for (size_t i = 0; i < p_Bodies.size(); i++)
	{
		p_Bodies[i].UpdateVelocity(p_DeltaTime);
		p_Bodies[i].UpdatePosition(p_DeltaTime);
	}
	s_CheckCollision(p_Bodies, p_PlaneBoundaries);

	Vec3 totalMomentum = Vec3(0.f, 0.f, 0.f);
	float totalEnergy = 0.f;
	for (const auto& ele : p_Bodies)
	{
		totalMomentum += ele.GetMomentum();
		totalEnergy += ele.GetTotalEnergy(p_PlaneBoundaries.y);
	}
}
