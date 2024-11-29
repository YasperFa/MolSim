//
// Created by Celik Yucel on 23.10.24.
//

    #include "DirectSumContainer.h"
    #include "spdlog/spdlog.h"
    #include <algorithm>
    #include <exception>

namespace ParticleContainers {
    std::vector<Particle>::iterator DirectSumContainer::begin() {
        return particles.begin();
    }
    std::vector<Particle>::iterator DirectSumContainer::end() {
        return particles.end();
    }

    void DirectSumContainer::addParticle(const Particle &particle) {
        SPDLOG_TRACE("adding particle to container");
        particles.push_back(particle);
    }

    void DirectSumContainer::removeParticle(const Particle &particle) {
        SPDLOG_TRACE("removing particle from container");

        // remove the particle itself from `particles`
        auto it = std::remove_if(
            particles.begin(),
            particles.end(),
            [&particle](const Particle &p) { return p.getID() == particle.getID(); });

        if (it != particles.end()) {
            particles.erase(it, particles.end());
        } else {
            SPDLOG_WARN("Attempted to remove a particle not found in container: ID {}", particle.getID());
        }
    }

    const Particle& DirectSumContainer::getParticle(int id) {

        for (auto &p:particles) { //linear search
            if (p.getID() == id){
                return p;
            }
        }
        
        SPDLOG_WARN("Particle not found");
        throw std::runtime_error("Particle not found");
    }

    const Particle& DirectSumContainer::getParticle(Particle& p) {
        return DirectSumContainer::getParticle(p.getID());
    }


    size_t DirectSumContainer::sizeParticles() const {
        return particles.size();
    }

    std::vector<Particle> DirectSumContainer::getParticles() const {
        return particles;
    }
}
