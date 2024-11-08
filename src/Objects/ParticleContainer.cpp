//
// Created by Celik Yucel on 23.10.24.
//

    #include "ParticleContainer.h"
    #include "spdlog/spdlog.h"
    #include <algorithm>

    std::vector<Particle>::iterator ParticleContainer::begin() {
        return particles.begin();
    }
    std::vector<Particle>::iterator ParticleContainer::end() {
        return particles.end();
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>>::iterator ParticleContainer::begin_pairs() {
        return particlePairs.begin();
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle> > >::iterator ParticleContainer::end_pairs() {
        return particlePairs.end();
    }



    void ParticleContainer::addParticle(const Particle &particle) {
        SPDLOG_TRACE("adding particle to container");
        particles.push_back(particle);
    }

    void ParticleContainer::removeParticle(const Particle &particle) {
        SPDLOG_TRACE("removing particle from container");
        particles.erase(
            std::remove_if(particles.begin(), particles.end(), [&particle](const Particle &p) {
                    if (&p == &particle) {
                        return true;
                    }
                    return false;
            }),
            particles.end());
        reinitializePairs();
    }

    bool ParticleContainer::pairExists(const Particle &particle1, const Particle &particle2) const {
        for (const auto &p: particlePairs) {
            if (((&p.first.get() == &particle1) && (&p.second.get() == &particle2)) ||
                ((&p.first.get() == &particle2) && (&p.second.get() == &particle1))) {
                return true;
            }
        }
        return false;
    }

    void ParticleContainer::initializePairsVector() {
        SPDLOG_DEBUG("Initializing pairs vector");
        for (auto iterator1 = particles.begin(); iterator1 != particles.end(); ++iterator1) {
            for (auto iterator2 = iterator1 + 1; iterator2 != particles.end(); ++iterator2) {
                if (!pairExists(*iterator1, *iterator2)) {
                    particlePairs.push_back({*iterator1,*iterator2});
                }
            }
        }
    }

    void ParticleContainer::reinitializePairs() {
        particlePairs.clear();
        initializePairsVector();
    }

    size_t ParticleContainer::sizeParticles() const {
        return particles.size();
    }

    size_t ParticleContainer::sizeParticlePairs() const {
        return particlePairs.size();
    }

    std::vector<Particle> ParticleContainer::getParticles() const {
        return particles;
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> ParticleContainer::getParticlePairs() const {
        return particlePairs;
    }



