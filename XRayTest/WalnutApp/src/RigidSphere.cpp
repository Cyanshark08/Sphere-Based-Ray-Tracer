#include "RigidSphere.h"
#include "glm/geometric.hpp"
#include "glm/glm.hpp"

#define COLLISION_SPACING_ERROR 0.01f

XRay::RigidSphere::RigidSphere()
	: Sphere()
{}

bool XRay::RigidSphere::WillBeOutOfBounds(const Vec3 & p_AmountToBeMoved, const Vec3& p_PlaneBoundaries)
{
	Vec3 newCenter = this->getCenter() + p_AmountToBeMoved;
	if (newCenter.y - this->getRadius() - p_PlaneBoundaries.y <= COLLISION_SPACING_ERROR)
		return true;
	if (fabs(newCenter.x - this->getRadius()) < p_PlaneBoundaries.x)
		return true;
	if (fabs(newCenter.z - this->getRadius()) < p_PlaneBoundaries.z)
		return true;
	return false;
}

bool XRay::RigidSphere::CheckYFloorCollision(float p_YFloorPlane) const
{
	//if (this->getCenter().y - this->getRadius() < p_YFloorPlane + COLLISION_SPACING_ERROR)
	//{
	//	return true;
	//}
	//else
	//{
	//	return false;
	//}

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
	//this->AddMomentum(-momentumTransfered - momentumTransfered * this->m_Elasticity);
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
	//printf("\nMomentum: %.2f", p_Bodies[i].GetMomentum().magnitude());
}

bool XRay::RigidSphere::CheckXWallCollision(float p_XWallPlane) const
{
	return (fabs(this->getCenter().x - this->getRadius()) < p_XWallPlane) ? false : true;
}

void XRay::RigidSphere::CollideBodyWithXWall(float p_XWallPlane)
{
	this->moveTo(Vec3((this->getCenter().x - this->getRadius() > p_XWallPlane ? p_XWallPlane - COLLISION_SPACING_ERROR : -p_XWallPlane + COLLISION_SPACING_ERROR) + this->getRadius(), this->getCenter().y, this->getCenter().z));
	Vec3 momentumTransfered = Vec3(1.f, 0.f, 0.f) * Vec3::dot(this->GetMomentum(), Vec3(1.f, 0.f, 0.f));
	//this->AddMomentum(-momentumTransfered - momentumTransfered * this->m_Elasticity);
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
	//printf("\nMomentum: %.2f", p_Bodies[i].GetMomentum().magnitude());
}

bool XRay::RigidSphere::CheckZWallCollision(float p_ZWallPlane) const
{
	return (fabs(this->getCenter().z - this->getRadius()) < p_ZWallPlane) ? false : true;
}

void XRay::RigidSphere::CollideBodyWithZWall(float p_ZWallPlane)
{
	this->moveTo(Vec3(this->getCenter().x, this->getCenter().y, (this->getCenter().z - this->getRadius() > p_ZWallPlane ? p_ZWallPlane - COLLISION_SPACING_ERROR : -p_ZWallPlane + COLLISION_SPACING_ERROR) + this->getRadius()));
	Vec3 momentumTransfered = Vec3(0.f, 0.f, 1.f) * Vec3::dot(this->GetMomentum(), Vec3(0.f, 0.f, 1.f));
	//this->AddMomentum(-momentumTransfered - momentumTransfered * this->m_Elasticity);
	this->AddMomentum(-momentumTransfered - momentumTransfered * m_Elasticity);
	//printf("\nMomentum: %.2f", p_Bodies[i].GetMomentum().magnitude());
}

bool XRay::RigidSphere::CheckOtherRigidBodyCollision(RigidSphere& p_OtherBody)
{
	//if (this->getRadius() * this->getRadius() )
	//{
	//	glm::normalize

	Vec3 dir = (this->getCenter() - p_OtherBody.getCenter());
	if (dir.magnitude() - (this->getRadius() + p_OtherBody.getRadius()) <= COLLISION_SPACING_ERROR)
	{
		return true;
	}
	return false;
	//XRay::Vec3::normalize(dir);
	return (abs(dir.magnitude() - (this->getRadius() + p_OtherBody.getRadius())) <= COLLISION_SPACING_ERROR) ? true : false;
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
	//printf("\n\tCollision occuring");
	//dir1 points from body 1 to body 2
	Vec3 dir1 = (p_Body2.getCenter() - p_Body1.getCenter());

	Vec3 dir1Norm = dir1;
	Vec3::normalize(dir1Norm);

	//dir2 points from body 2 to body 1
	Vec3 dir2Norm = -dir1Norm;

	float centerDistance = dir1.magnitude();

	//printf("tranfered\n");

	float dot1 = Vec3::dot(p_Body1.GetMomentum(), dir1Norm);

		//Vec3 MinimumPositionFrom2To1 = dir2Norm * (p_Body1.getRadius() + p_Body2.getRadius());
		//if (fabs(p_Body2.getCenter().x + MinimumPositionFrom2To1.x) < p_PlaneBoundaries.x &&
		//	p_Body2.getCenter().y + MinimumPositionFrom2To1.y > p_PlaneBoundaries.y &&
		//	fabs(p_Body2.getCenter().z + MinimumPositionFrom2To1.z) < p_PlaneBoundaries.z)
		//{
		//	float diff = p_Body1.getRadius() + p_Body2.getRadius() - dir1.magnitude();
		//	p_Body1.moveBy(dir2Norm * diff);
		//}

	float sphereDistance = centerDistance - p_Body2.getRadius() - p_Body1.getRadius();

	//printf("\n\tSphere distance: %.2f", sphereDistance);

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

	Vec3 momentumTransferedBy1 = dir1Norm * p_Body1.m_Elasticity;
	//printf("\nOld Momentum:\t");
	//p_Body1.GetMomentum().printVector();
	//printf("\nNew Momentum:\t");
	//p_Body1.GetMomentum().printVector();



	//Momentum transfered to body 1 by body 2 and body 2 to body 1

	float dot2 = Vec3::dot(p_Body2.GetMomentum(), dir2Norm);
		Vec3 momentumTransferedBy2 = dir2Norm * p_Body2.m_Elasticity;

		p_Body1.AddMomentum(-momentumTransferedBy1);
		p_Body2.AddMomentum(momentumTransferedBy1);

		p_Body2.AddMomentum(-momentumTransferedBy2);
		p_Body1.AddMomentum(momentumTransferedBy2);
}

void XRay::RigidSphere::s_UpdatePhysics(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries, float p_DeltaTime)
{
	for (size_t i = 0; i < p_Bodies.size(); i++)
	{
		p_Bodies[i].UpdatePosition(p_DeltaTime);
		p_Bodies[i].UpdateVelocity(p_DeltaTime);
		//printf("\n\nAcc XYZ: %.2f, %.2f, %.2f ", p_Bodies[i].m_Acceleration.x, p_Bodies[i].m_Acceleration.y, p_Bodies[i].m_Acceleration.z);
		//printf("\nVel XYZ: %.2f, %.2f, %.2f ", p_Bodies[i].m_Velocity.x, p_Bodies[i].m_Velocity.y, p_Bodies[i].m_Velocity.z);
		//printf("\nMomentum: %.2f", p_Bodies[i].GetMomentum().magnitude());
	}
	s_CheckCollision(p_Bodies, p_PlaneBoundaries);
}
