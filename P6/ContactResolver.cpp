#include "ContactResolver.h"
#include "ParticleContact.h"
#include <iostream>

using namespace physics;

void ContactResolver::ResolveContacts(std::vector<ParticleContact*> contacts, float time)
{
    // If there are no contacts, return immediately
    if (contacts.empty()) {
        return;
    }
    current_iteration = 0;

    // Iterate until the maximum number of iterations is reached
    while (current_iteration < max_iterations) {
        unsigned int curr_index = 0; // Index of the contact with the minimum separating speed
        float minSeparatingSpeed = contacts[0]->GetSeparatingSpeed(); // Minimum separating speed initialized
        float max_depth = contacts[0]->depth; // Maximum penetration depth initialized

        // Find the contact with the smallest separating speed (most negative)
        for (unsigned i = 0; i < contacts.size(); i++) {
            float separatingSpeed = contacts[i]->GetSeparatingSpeed();

            // Update the current index and minimum separating speed if the current contact has a smaller separating speed
            if (separatingSpeed <= minSeparatingSpeed && separatingSpeed < 0) {
                curr_index = i;
                minSeparatingSpeed = separatingSpeed;

                // Update the maximum depth if the current contact has a greater depth
                if (max_depth < contacts[i]->depth) {
                    max_depth = contacts[i]->depth;
                }
            }
        }

        // If the smallest separating speed is non-negative and maximum depth is non-positive, stop resolving
        if (minSeparatingSpeed >= 0 && max_depth <= 0) {
            break;
        }

        // Resolve the contact with the smallest separating speed
        contacts[curr_index]->Resolve(time);
        current_iteration++;
    }
}
