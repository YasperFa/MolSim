//
// Created by Celik Yucel on 23.10.24.
//

    #include "DirectSumContainer.h"
    #include "spdlog/spdlog.h"
    #include <algorithm>
    #include <exception>

    std::vector<Particle>::iterator DirectSumContainer::begin() {
        return particles.begin();
    }
    std::vector<Particle>::iterator DirectSumContainer::end() {
        return particles.end();
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>>::iterator DirectSumContainer::begin_pairs() {
        return particlePairs.begin();
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle> > >::iterator DirectSumContainer::end_pairs() {
        return particlePairs.end();
    }

    DirectSumContainer::~DirectSumContainer() = default;

    void DirectSumContainer::addParticle(const Particle &particle) {
        SPDLOG_TRACE("adding particle to container");
        particles.push_back(particle);
    }

     void DirectSumContainer::addParticleToPairs(Particle& particle) {
        for ( auto & p : particles) {
            std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>> pair = std::make_pair(std::ref(p), std::ref(particle));
            particlePairs.push_back(pair);
            //SPDLOG_TRACE("Pair created! {},{}" ,p.getID(), particle.getID());
        }
        
        particles.push_back(particle);
     }

    void DirectSumContainer::removeParticle(const Particle &particle) {
        SPDLOG_TRACE("removing particle from container");

        particlePairs.erase(
            std::remove_if(begin_pairs(), end_pairs(), [&particle] (const std::pair<std::reference_wrapper<Particle>, std::reference_wrapper <Particle>> &pair) {
                    if  ((pair.first.get().getID() == particle.getID())|| (pair.second.get().getID() == particle.getID()) ) {
                        //SPDLOG_TRACE("removing pair {},{}", pair.first.get().getID(),pair.second.get().getID( ));
                        return true;
                    }
                    return false;
            }),
            end_pairs());

        particles.erase(
            std::remove_if(particles.begin(), particles.end(), [&particle](const Particle &p) {
                    if (p.getID() == particle.getID()) {
                        return true;
                    }
                    return false;
            }),
            particles.end());
    }

    Particle& DirectSumContainer::getParticle(int id) {
        if ((int)particles.size() < id + 1) { //linear search
            for (auto &p:particles) {
                if (p.getID() == id){
                    return p;
                }
            }
        } else { //jump to pos id and compare, move to left if ids don't match (particle with smaller id has been deleted)
            int i = id;
            while (particles.at(i).getID() != id) {
                i--;
            }
            return particles.at(i);
        }

        for (auto &p:particles) { //last resort: linear search in case particles not sorted
                if (p.getID() == id){
                    return p;
                }
            }
        
        SPDLOG_WARN("Particle not found");
        throw std::runtime_error("Particle not found");
    }

    Particle& DirectSumContainer::getParticle(Particle& p) {
        return DirectSumContainer::getParticle(p.getID());
    }


    bool DirectSumContainer::pairExists(const Particle &particle1, const Particle &particle2) const {
        for (const auto &p: particlePairs) {
            if (((&p.first.get() == &particle1) && (&p.second.get() == &particle2)) ||
                ((&p.first.get() == &particle2) && (&p.second.get() == &particle1))) {
                return true;
            }
        }
        return false;
    }

    void DirectSumContainer::initializePairsVector() {
        SPDLOG_DEBUG("Initializing pairs vector");
        for (auto iterator1 = particles.begin(); iterator1 != particles.end(); ++iterator1) {
            for (auto iterator2 = iterator1 + 1; iterator2 != particles.end(); ++iterator2) {
                if (!pairExists(*iterator1, *iterator2)) {
                    particlePairs.push_back({*iterator1,*iterator2});
                }
            }
        }
    }

    void DirectSumContainer::reinitializePairs() {
        particlePairs.clear();
        initializePairsVector();
    }

    size_t DirectSumContainer::sizeParticles() const {
        return particles.size();
    }

    size_t DirectSumContainer::sizeParticlePairs() const {
        return particlePairs.size();
    }

    std::vector<Particle> DirectSumContainer::getParticles() const {
        return particles;
    }

    std::vector<std::pair<std::reference_wrapper<Particle>, std::reference_wrapper<Particle>>> DirectSumContainer::getParticlePairs() const {
        return particlePairs;
    }



