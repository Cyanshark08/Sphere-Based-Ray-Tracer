#pragma once
#include "Sphere.h"

namespace XRay
{

	class Pendulum
	{
	public:
		void Update(float p_Delta);
		void UpdateAngularAcceleration(float p_Delta);
		void UpdateAngularVelocity(float p_Delta);
		void UpdateAngle(float p_Delta);
		void UpdateBody();

		const Vec3& GetPivot() const;

		const Sphere& getObject() const;

	private:
		Sphere m_Object;
		Sphere m_Pivot;
		float m_StringLength;
		float m_AngularAcceleration;
		float m_AngularVelocity;
		float m_CurrentAngle;


	};

}
