#pragma once

#include "ForceGenerator.h"

namespace physics
{
    class Chain : public ForceGenerator
    {
    private:
        //Point in the world where the other end of the spring is
        MyVector anchorPoint;
        //Spring constant
        //How "stiff" this spring is
        float springConstant;
        //Rest length of the spring
        float restLength;
    public:
        Chain(MyVector pos, float springConstant, float restLength) :
            anchorPoint(pos), springConstant(springConstant), restLength(restLength) {}
        virtual void UpdateForce(PhysicsParticle* particle, float time) override;
    };
}


