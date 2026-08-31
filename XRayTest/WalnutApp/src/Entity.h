#pragma once
#include <cstdint>

#include "Ray.h"
#include "Vec3.h"

namespace XRay
{
	class Entity
	{
	public:
		Entity() = default;

		virtual const Vec3& getCenter() const = 0;
		virtual const Vec3& getColor() const = 0;

		virtual const Vec3& moveBy(const Vec3& p_Increment) = 0;
		virtual const Vec3& moveTo(const Vec3& p_NewPosition) = 0;

		virtual float getIntersectionDistance(const Ray& p_Ray) = 0;
		virtual Vec3 getIntersectionColor(const Vec3& p_LightDirection, const Vec3& p_Norm, const Vec3& p_BackgroundColor, const Ray& p_Ray) = 0;

		uint8_t GetEntityID() const { return m_EntityID; }

	private:
		uint8_t m_EntityID;
	};

}


