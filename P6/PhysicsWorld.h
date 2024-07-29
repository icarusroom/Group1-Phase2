#pragma once
#include <list>
#include "Force/ForceRegistry.h"
#include "Force/GravityForceGenerator.h"
#include "ContactResolver.h"
#include "ParticleLink.h"
#include <vector>


namespace physics {
	class ParticleContact;

	class PhysicsWorld
	{
	public:
		std::list<ParticleLink*> Links;

		//The list of ALL our particles
		std::list<PhysicsParticle*> Particles;

		//Function to add particles to the list
		void AddParticle(PhysicsParticle* toAdd);

		//Universal update function to call the updates of All
		void Update(float time);

		ForceRegistry forceRegistry;

	private:
		//Updates the particle list
		void UpdateParticleList();

		GravityForceGenerator Gravity;

	public:
		// Constructor to initialize gravity with user input
		MyVector gravity;
		PhysicsWorld(const MyVector& gravity) : Gravity(GravityForceGenerator(gravity)), gravity(gravity) {}
		//Create a Dynamic Array of Contacts
		//You can use the vector cass for this
		std::vector<ParticleContact*> Contacts;
		//Function to easily add contacts
		void AddContact(PhysicsParticle* p1, PhysicsParticle* p2, float restitution, MyVector contactNormal, float depth);
		void GenerateContacts();

	protected:
		ContactResolver contactResolver = ContactResolver(20);
		void GetOverlaps();
	};
}


