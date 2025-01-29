//
// Created by Celik Yucel on 24.11.24.
//
#pragma once
#include "Objects/Cell.h"
#include "Objects/Containers/ParticleContainer.h"

namespace ParticleContainers {
 /**
  * @brief Container for LinkedCell algorithm for improving the performance of the DirectSum algorithm.
  * Extends ParticleContainer
  */
 class LinkedCellContainer : public ParticleContainer {
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
   * @brief A vector to store iteration orders.
   */
  std::vector<std::vector<Cell*>> iterationOrders;
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
  std::vector<std::reference_wrapper<Cell> > innerCells;

  /**
   * @brief Vector containing references to boundary cells in the domain
   */
  std::vector<std::reference_wrapper<Cell> > boundaryCells;

  /**
  * @brief Vector containing references to halo cells outside the domain
  */
  std::vector<std::reference_wrapper<Cell> > haloCells;

  /**
   * @brief An array containing cell sizes for every dimension
   */
  std::array<double, 3> cellSizePerDimension{};

  /**
   * @brief An array containing number of cells for every dimension
   */
  std::array<int, 3> cellNumPerDimension{};


  /**
   * @brief initialize the cell vector and set the cell types
   */
  void initializeCells();

  /**
   * @brief Set the neighbouring cells for every cell in the cells vector
   */
  void initializeNeighbours();



 public:
  /**
   * @brief Constructor for a new Linked Cell Container
   * @param domainSize size of the domain that will be used for the simulation
   * @param cutoff cut off radius for the simulation
   * @param version2 whether or not to use parallelization version 2 (if false: use version 1)
   */
  LinkedCellContainer(const std::array<double, 3> &domainSize, const double cutoff, const bool version2);

  /**
   * indicates whether version 2 of parallelization is enabled
   */
  const bool version2;
  /**
   * @brief Add the particle passed in to the container.
   * @param particle is the particle to be added.
   */
  void addParticle(const Particle &particle) override;

  /**
   * @brief Remove the particle passed from the container.
   * @param particle is the particle to be removed
  */
  void removeParticle(const Particle &particle);

  /**
   * @brief allocates memory for the given number of particles
   * @param numParticles number of particles
   */
  void reserve(size_t numParticles);


  /**
  * @return an iterator to the beginning of the particles vector.
  */
  std::vector<Particle>::iterator begin() override;

  /**
  * @return an iterator to the end of the particles vector.
  */
  std::vector<Particle>::iterator end() override;

  /**
  * @return an iterator to the beginning of the cells vector.
  */
  std::vector<Cell>::iterator beginCells();

  /**
  * @return an iterator to the end of the cells vector.
  */
  std::vector<Cell>::iterator endCells();

  /**
 * @return an iterator to the beginning of the inner cells vector.
 */
  std::vector<std::reference_wrapper<Cell> >::iterator beginInnerCells();

  /**
  * @return an iterator to the end of the inner cells vector.
  */
  std::vector<std::reference_wrapper<Cell> >::iterator endInnerCells();


  /**
    * @return an iterator to the beginning of the boundary cells vector.
    */
  std::vector<std::reference_wrapper<Cell> >::iterator beginBoundaryCells();


  /**
    * @return an iterator to the end of the inner boundary vector.
    */
  std::vector<std::reference_wrapper<Cell> >::iterator endBoundaryCells();


  /**
    * @return an iterator to the beginning of the halo cells vector.
    */
  std::vector<std::reference_wrapper<Cell> >::iterator beginHaloCells();

  /**
    * @return an iterator to the end of the halo cells vector.
    */
  std::vector<std::reference_wrapper<Cell> >::iterator endHaloCells();

  /**
    * @return a vector containing the particles.
    */
  std::vector<Particle> getParticles() const override;

  /**
   *
   * @return size of the particles vector
   */
  size_t sizeParticles() const override;

  /**
   *
   * @return a vector containing the cells
   */
  std::vector<Cell> & getCells();

  /**
   *
   * @return the size of the cells vector
   */
  size_t sizeCells() const;

  /**
   *
   * @return a vector containing references to the inner cells.
   */
  std::vector<std::reference_wrapper<Cell>> getInnerCells();

  /**
  *
  * @return a vector containing references to the boundary cells.
  */
  std::vector<std::reference_wrapper<Cell>> getBoundaryCells();

  /**
   *
   * @return a vector containing references to the halo cells.
   */
  std::vector<std::reference_wrapper<Cell>> getHaloCells();
  /**
   *
   * @return a vector containing iteration orders.
   */
  std::vector<std::vector<Cell*>> getIterationOrders();
  /**
   * @brief Getter for the cutoff radius.
   * @return cutoff radius
   */
  double getCutoff() const;

  /**
   * @brief Getter for domain size
   * @return an array containing the domain size
   */
  std::array<double, 3> getDomainSize() const;

  /**
   *
   * @return an array containing the sizes of cells for each dimension.
   */
  std::array<double, 3> getCellSizePerDimension() const;

  /**
   *
   * @return an array containing the number of cells for each dimension.
   */
  std::array<int, 3> getCellNumPerDimension() const;

  /**
   * @brief Computes which cell the passed particle should be added into based on its position
   * @param particle is the particle to be added
   * @return a reference to the Cell which the particle belongs to
   */
  Cell* mapParticleToCell(const Particle &particle);


  /**
   * @brief Updates all cells according to changed particle positions*/
  void updateParticlesInCell();

  /**
   * @brief computes the index of the passed cell based on its position in the cells grid.
   * @param x
   * @param y
   * @param z
   * @returns index of the cell in the cells vector
   */
  int cellIndex(int x, int y, int z) const;
  /**
   * @brief initialize parallelV2 iteration*/
  void initParallelV2();


 };
}