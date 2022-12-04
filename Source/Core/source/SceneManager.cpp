#include "pch.h"
#include "SceneManager.h"

namespace Render
{
    World::World()
    {
        mCollisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
        mDispathcer = std::make_unique<btCollisionDispatcher>(mCollisionConfig.get());
        mBroadphase = std::make_unique<btDbvtBroadphase>();
        mSolver = std::make_unique<btSequentialImpulseConstraintSolver>();
        mDynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(mDispathcer.get(), mBroadphase.get(), mSolver.get(), mCollisionConfig.get());
        mDynamicsWorld->setGravity(btVector3(0.f, -9.81f, 0.f));
    }

    void World::Spawn(Core::BaseEntity* pEntity)
    {
        glm::quat qRotation = glm::quat(pEntity->GetRotation());
        glm::vec3 position = pEntity->GetPosition();
        btDefaultMotionState* pMotionState = new btDefaultMotionState(btTransform(
            btQuaternion(qRotation.x, qRotation.y, qRotation.z, qRotation.w),
            btVector3(position.x, position.y, position.z)
        ));

        btCollisionShape* pShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));


        btRigidBody::btRigidBodyConstructionInfo rigidBodyCInfo(
            0,
            pMotionState,
            pShape,
            btVector3(0.0f, 0.0f, 0.0f)
        );
        mDynamicsWorld->addRigidBody(new btRigidBody(rigidBodyCInfo));
    }

    World& World::Get()
    {
        static World instance;
        return instance;
    }
}