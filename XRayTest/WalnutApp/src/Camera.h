#pragma once
#include "Vec3.h"

namespace XRay
{

	class Camera
	{
	public:
		Camera() : m_Position(), m_Pitch(0.f), m_Yaw(0.f), m_Roll(0.f) {}
		Camera(Vec3& p_Origin) : m_Position(p_Origin), m_Pitch(0.f), m_Yaw(0.f), m_Roll(0.f) {}
		Camera(float p_Pitch, float p_Yaw, float p_Roll) : m_Position(), m_Pitch(p_Pitch), m_Yaw(p_Yaw), m_Roll(p_Roll) {}
		Camera(Vec3& p_Origin, float p_Pitch, float p_Yaw, float p_Roll) : m_Position(p_Origin), m_Pitch(p_Pitch), m_Yaw(p_Yaw), m_Roll(p_Roll) {}

		float getRoll() const;
		float getPitch() const;
		float getYaw() const;
		const Vec3& getPosition() const;

		void OnUpdate();
		//bool hasChanged() const;

	private:
		Vec3 m_Position;
		Vec3 m_Direction;
		float m_Pitch, m_Yaw, m_Roll;

	};

}
