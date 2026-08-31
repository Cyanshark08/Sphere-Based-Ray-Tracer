#pragma once
#include "Vec3.h"

namespace XRay {

	class Ray
	{
	public:
		Ray();
		Ray(const Vec3& p_Direction, const Vec3& p_Origin);

		const Vec3& getDirection() const;
		const Vec3& getOrigin() const;

		const Vec3& setDirection(const Vec3& p_NewDirection);
		const Vec3& setOrigin(const Vec3& p_NewOrigin);

	private:
		Vec3 m_Direction;
		Vec3 m_Origin;
	};

}
