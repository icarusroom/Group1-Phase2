#include "PhysicsWorld.h"
#include "PhysicsParticle.h"
#include "ParticleContact.h"

using namespace physics;

void PhysicsWorld::AddParticle(PhysicsParticle* toAdd)
{
	Particles.push_back(toAdd);

	forceRegistry.Add(toAdd, &Gravity);
}

void PhysicsWorld::Update(float time)
{
	UpdateParticleList();

	forceRegistry.UpdateForces(time);
	
	for (std::list<PhysicsParticle*>::iterator p = Particles.begin(); p != Particles.end(); p++) {
		(*p)->update(time);
	}

	GenerateContacts();

	if (Contacts.size() > 0)
	{
		contactResolver.ResolveContacts(Contacts, time);
	}
}

void PhysicsWorld::UpdateParticleList()
{
	Particles.remove_if([](physics::PhysicsParticle* p) {
		return p->IsDestroyed();
		}
	);
}

void PhysicsWorld::AddContact(PhysicsParticle* p1, PhysicsParticle* p2, float restitution, MyVector contactNormal, float depth)
{
	//Create a Particle Contact
	ParticleContact* toAdd = new ParticleContact();

	//Assign the needed variables and values
	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->contactNormal = contactNormal;
	toAdd->depth = depth;

	//Similar to a ist you can just call push_back
	Contacts.push_back(toAdd);
}

void PhysicsWorld::GetOverlaps() {
	for (int i = 0; i < Particles.size() - 1; i++) {
		std::list<PhysicsParticle*>::iterator a = std::next(Particles.begin(), i);

		for (int h = i + 1; h < Particles.size(); h++) {
			std::list<PhysicsParticle*>::iterator b = std::next(Particles.begin(), h);

			MyVector mag2Vector = (*a)->Position - (*b)->Position;
			float mag2 = mag2Vector.SquareMagnitude(); //square magnitude
			float rad = (*a)->radius + (*b)->radius;
			float rad2 = rad * rad;

			if (mag2 <= rad2) {
				MyVector dir = mag2Vector.direction(); //normalize

				float r = rad2 - mag2;
				float depth = sqrt(r);

				float restitution = fmin(
					(*a)->restitution, (*b)->restitution
				);

				AddContact(*a, *b, restitution, dir, depth);
			}
		}
	}
}


void PhysicsWorld::GenerateContacts()
{
	//Clear the current list of contacts;
	Contacts.clear();

	//Check for overlaps
	GetOverlaps();

	for (std::list<ParticleLink*>::iterator i = Links.begin(); i != Links.end(); i++)
	{
		ParticleContact* contact = (*i)->GetContact();
		if (contact != nullptr)
		{
			Contacts.push_back(contact);
		}
	}
}
