//
// Created by Celik Yucel on 24.11.24.
//
#pragma once
#include "Objects/Cell.h"
#include "Objects/Containers/ParticleContainer.h"

/**
 * @brief Container for LinkedCell algorithm for improving the performance of the DirectSum algorithm.
 * Extends ParticleContainer
 */
class LinkedCell : public ParticleContainer {

private:
 /**
  * @brief A vector to store Particle objects.
  */
 std::vector<Particle> particles;

 /**
 * @brief A vector to store Cell objects.
 */
 std::vector<Cell> cells;

 /**
  * @brief An array of size 3 to save the domain size. The size is set to 3 to allow 3D simulation.
  */
 std::array<double, 3> domainSize;

 /**
  * @brief cutoff radius for the algorithm
  */
 double cutoff;

 /**
  * @brief Vector containing references to inner cells in the domain
  */
 std::vector<std::reference_wrapper<Cell>> innerCells;

 /**
  * @brief Vector containing references to boundary cells in the domain
  */
 std::vector<std::reference_wrapper<Cell>> boundaryCells;

 /**
 * @brief Vector containing references to halo cells outside the domain
 */
 std::vector<std::reference_wrapper<Cell>> haloCells;

 /**
  * @brief An array containing cell sizes for every dimension
  */
 std::array<double, 3> cellSizePerDimension;

 /**
  * @brief An array containing number of cells for every dimension
  */
 std::array<double, 3> cellNumPerDimension;


 /**
  * @brief initialize the cell vector and set the cell types
  */
 void initializeCells();

 /**
  * @brief Set the neighbouring cells for every cell in the cells vector
  */
 void initializeNeighbours();

 /**
  * @brief Remove particles that are contained in halo cells from the particle container.
  */
 void deleteHaloParticles();


public:
 /**
  * @brief Constructor for a new Linked Cell Container
  * BOUNDARIES FROM TASK 3 HAS TO BE ADDED!!!
  * @param domainSize size of the domain that will be used for the simulation
  * @param cutoff cut off radius for the simulation
  */
 LinkedCell(const std::array<double, 3> domainSize, const double cutoff);

 /**
  * @brief Add the particle passed in to the container
  * @param particle is the particle to be added.
  */
 void addParticle(const Particle &particle) override;

};