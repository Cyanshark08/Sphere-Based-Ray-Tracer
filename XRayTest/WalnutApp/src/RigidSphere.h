#pragma once
#include "Sphere.h"
#include <vector>
#include "glm/vec3.hpp"
#include "glm/common.hpp"
#include "glm/vector_relational.hpp"


namespace XRay {

    class RigidSphere :
        public Sphere
    {
    public:
        RigidSphere();

        bool WillBeOutOfBounds(const Vec3& p_AmountToBeMoved, const Vec3& p_PlaneBoundaries);

        bool CheckYFloorCollision(float p_YFloorPlane) const;
        void CollideBodyWithFloor(float p_YFloorPlane);

        bool CheckXWallCollision(float p_XWallPlane) const;
        void CollideBodyWithXWall(float p_XWallPlane);

        bool CheckZWallCollision(float p_ZWallPlane) const;
        void CollideBodyWithZWall(float p_ZWallPlane);

        bool CheckOtherRigidBodyCollision(RigidSphere& p_OtherBody);

        Vec3 GetMomentum() const;
        void AddMomentum(Vec3 p_MomentumTransfered);
        void SetMomentum(Vec3 p_Momentum);

        void UpdatePosition(float p_DeltaTime);
        void UpdateVelocity(float p_DeltaTime);

        float* GetMutableElasticity();

    //static
        static void s_CheckCollision(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries);
        static void s_CollideAllRigidBodies(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries);
        static void s_Collide(RigidSphere& p_Body1, RigidSphere& p_Body2, const Vec3& p_PlaneBoundaries);
        static void s_UpdatePhysics(std::vector<RigidSphere>& p_Bodies, const Vec3& p_PlaneBoundaries, float p_DeltaTime);

    private:
        float m_Elasticity = 1.f;
        float m_Mass = 1.f;
        Vec3 m_Velocity = Vec3(0.f, 0.f, 0.f);
        Vec3 m_Acceleration = Vec3(0.f, -9.81f, 0.f);

    };

}

