//
// Created by Yasmine Farah on 05/12/2024.
//

#pragma once
#include "Objects/Containers/ParticleContainer.h"
#include "utils/ArrayUtils.h"
#include <functional>
/**
 * @brief control the temperature of a particle container.
 */
class Thermostat{

protected:
 /**
  * @brief The target temperature for thermostat.
 */
 const double targetTemperature;
 /**
  * @brief maximum temperature change per application
 */
 const double maxDeltaT;
 /**
  * @brief number of dimensions is three
 */
 const bool is3D;
 /**
  * @brief number of timesteps after which the thermostat is applied
 */
 const int n;
public:

 Thermostat(const double targetTemperature, const double maxDeltaT,const bool is3D, const int n) : targetTemperature(targetTemperature), maxDeltaT(maxDeltaT), is3D(is3D), n(n){};

 virtual ~Thermostat() = default;
/**
 * @brief applies the desired thermostat to all particles in the container
 * @param particleContainer: Particle container
*/
 virtual void applyThermostat(ParticleContainers::ParticleContainer& particleContainer) = 0;
 /**
 * @brief calculates the kinetic energy of the container
 * @param particleContainer: Particle container
 * @return kinetic energy of the container
 */
 static double getContainerKineticEnergy(ParticleContainers::ParticleContainer& particleContainer) {
    double total = 0;
    for (auto& particle : particleContainer) {
     std::array<double, 3> v = particle.getV();
     total += particle.getM() * (v[0] * v[0] + v[1] * v[1] + v[2] * v[2]) * 0.5;
    }
     return total;
  }
 /**
 * @brief calculates the average velocity of particles in the container
 * @param particleContainer: Particle container
 * @return Average velocity
 */
 static std::array<double, 3> getAverageVelocity(ParticleContainers::ParticleContainer& particleContainer) {
   std::array<double, 3> averageVelocity = {0, 0, 0};
   size_t count = 0;
   for (auto& particle : particleContainer) {
      averageVelocity = averageVelocity + particle.getV();
      count++;
   }
    averageVelocity = operator*((1.0 / count) , averageVelocity);
    return averageVelocity;
 }
 /**
* @return number of timesteps
*/
 int getNtimeSteps() const{
  return n;
 }
 /**
* @return number of timesteps
*/
 static double getCurrentTemperature(ParticleContainers::ParticleContainer& particleContainer, const bool is3D)  {
  const int dimension = is3D ? 3 : 2;
  return 2 * getContainerKineticEnergy(particleContainer) / (dimension * particleContainer.sizeParticles());
 }

};