//
// Created by Celik Yucel on 15.01.25.
//

#include "HarmonicForceCalculator.h"
#include "utils/ArrayUtils.h"
#include <cmath>
#include "spdlog/spdlog.h"

namespace Calculators {
    HarmonicForceCalculator::HarmonicForceCalculator(const ParticleContainers::ParticleContainer &container) {
        // Check the container type using dynamic casting
        const auto *linkedCellContainer = dynamic_cast<const ParticleContainers::LinkedCellContainer *>(&container);

        if (linkedCellContainer) {
            isLinkedCellContainer = true;
            domainSize = linkedCellContainer->getDomainSize();
        } else {
            isLinkedCellContainer = false;
        }
    }

    std::array<double, 3> HarmonicForceCalculator::calculateForce(Particle &current) const {
        auto totalHarmonicForce = std::array<double, 3>{0.0, 0.0, 0.0};

        // Iterate over all neighbor particles
        for (const auto &neighbor : current.getNeighbourParticles()) {
            // Access elements of the tuple explicitly for C++14 compatibility
            auto ptrDiff = std::get<0>(neighbor);
            auto l0 = std::get<1>(neighbor);
            auto k = std::get<2>(neighbor);

            Particle *connectedParticle = &current + ptrDiff;
            auto displacement = connectedParticle->getX() - current.getX();

            if (isLinkedCellContainer) {
                for (size_t i = 0; i < displacement.size(); ++i) {
                    if (std::abs(displacement[i]) > domainSize[i] * 0.5) {
                        displacement[i] += (displacement[i] > 0) ? -domainSize[i] : domainSize[i];
                    }
                }
            }

            // Calculate the distance and force
            double distance = ArrayUtils::L2Norm(displacement);
            double harmonicForce = k * (distance - l0) / distance;
            totalHarmonicForce = totalHarmonicForce + harmonicForce * displacement;
        }

        return totalHarmonicForce;
    }

    std::string HarmonicForceCalculator::toString() {
        return "HarmonicForceCalculator";
    }
}
