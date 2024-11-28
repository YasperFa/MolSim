#include "LinkedCellContainer.h"
#include <list>

class BoundaryHandler{

    private:
    /** the sigma value used for calculating the Lennard-Jones Potential */
    double sigma;
    /** type 0: Outflow
     *  type 1: Reflecting
     */
    const bool type; 
    /** ParticleContainer that the BoundaryHandler operates on */
    LinkedCellContainer & container;

    /**minimal distance between two particles to be repulsing each other*/
    const double minDist;

    /**boundaries of the simulation: left, right, top, bottom(, front, back) */
    const std::array<double, 6> boundaries;

    void handleOutflow();

    void handleReflecting();

    public:

    BoundaryHandler(double s, bool t, LinkedCellContainer& pc);
    ~BoundaryHandler() = default;

    void handleBoundaries();

    double calculateDistance(Particle p, int i);

    void getHaloNeighbors(Cell& c, std::list<Cell*> list);

    Particle createShadowParticle(Particle p, int i, double dist);

};