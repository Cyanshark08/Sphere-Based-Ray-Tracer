#pragma once
#include "Vec3.h"
#include "Ray.h"
#include "Entity.h"

namespace XRay {

	class Sphere : public Entity
	{
	public:
		Sphere();

		const Vec3& getCenter() const override;
		const Vec3& getColor() const override;
		const float getRadius() const;

		Vec3* getMutableCenter();
		Vec3* getMutableColor();
		float* getMutableRadius();

		const Vec3& moveBy(const Vec3& p_Increment) override;
		const Vec3& moveTo(const Vec3& p_NewPosition) override;

		float getIntersectionDistance(const Ray& p_Ray) override;
		Vec3 getIntersectionColor(const Vec3& p_LightDirection, const Vec3& p_Norm, const Vec3& p_BackgroundColor, const Ray& p_Ray) override;

	private:
		Vec3 m_Center;
		Vec3 m_Color;
		float m_Radius;
	};

}


