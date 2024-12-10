#pragma once
//
// Created by Stefanie Blattenberger on 28/11/2024.
//

/**Namespace that provides functionality for creating unique particle ids */
namespace ParticleIdInitializer {

/**Returns a new unique id. The ids start with 1 and are returned in ascending order.
 * Guarantees unique ids even if several particles are created parallelly.
 * @returns unique int id for a new particle
 */
    int getNewId();

/**Resets the ParticleIdInitializer to id = 1
 * Only used for testing purposes
 */
    void reset();
}
