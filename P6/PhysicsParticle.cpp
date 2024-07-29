#include "PhysicsParticle.h"
#include <random>
#include <iostream>

using namespace physics;

physics::PhysicsParticle::PhysicsParticle()
{

}

void PhysicsParticle::UpdatePosition(float time)
{
	float newTime = time * time;
	this->Position = this->Position + (this->Velocity.ScalarMultiplication(time)) + ((this->Acceleration.ScalarMultiplication(newTime)).ScalarMultiplication(0.5f));
}

void PhysicsParticle::UpdateVelocity(float time)
{
	this->Acceleration += accumulatedForce.ScalarMultiplication(1 / mass);
	this->Velocity = this->Velocity + (this->Acceleration.ScalarMultiplication(time));

	this->Velocity = this->Velocity.ScalarMultiplication(powf(damping, time));
}

void PhysicsParticle::AddForce(MyVector force)
{
	this->accumulatedForce += force;
}

void physics::PhysicsParticle::ResetForce()
{
	this->accumulatedForce = MyVector(0, 0, 0);
	this->Acceleration = MyVector(0, 0, 0);
}

void PhysicsParticle::update(float time)
{
	if (this->isAffected == false) return;
	this->UpdatePosition(time);
	this->UpdateVelocity(time);
	this->ResetForce();
}

void PhysicsParticle::Destroy()
{
	this->isDestroyed = true;
}
