#pragma once
#include "MyVector.h"

namespace physics{

	class PhysicsParticle
	{
		protected:
			bool isDestroyed = false;
			bool isAffected = true;
			MyVector accumulatedForce = MyVector(0, 0, 0);
		public:
			float mass = 0;
			MyVector Velocity;
			MyVector Position;
			MyVector Acceleration;

			float damping = 0.9f;

			float radius;
			float restitution = 0.9f;

		protected:
			void UpdatePosition(float time);
			void UpdateVelocity(float time);

		public:
			PhysicsParticle();
			void update(float time);
			void AddForce(MyVector force);
			void ResetForce();

		public:
			void Destroy();
			bool IsDestroyed() { return isDestroyed; }
			MyVector GetPosition() { return Position; }
			bool GetAffected() { return this->isAffected; }
			void SetAffected(bool force) { this->isAffected = force; };

	};
}

