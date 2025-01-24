//
// Created by Stefanie Blattenberger on 01/12/2024.
//

#include "gtest/gtest.h"
#include "../../src/Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include "../../src/Objects/Containers/LinkedCell/BoundaryHandler.h"
#include "../../src/Objects/ParticleIdInitializer.h"
#include "../../src/Calculator/LennardJonesCalculator.h"

/**Test the function handleOutflow() from BoundaryHandler */
TEST(BoundaryHandlerTest, conditionOutflow) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1, false);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({2, 3, 0}, {0, 1.1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -1.1, 0}, 1, 0));
testContainer.addParticle(Particle({1, 2, 0}, {-1.1, 0, 0}, 1, 0));
testContainer.addParticle(Particle({3, 2, 0}, {1.1, 0, 0}, 1, 0));


EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.5,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

for (int i = 1; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particles are still in boundary cells
}

calculator.calculateX(testContainer, 0.5);
testContainer.updateParticlesInCell();

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

for (int i = 1; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particles are in halo cells
    
}

for (auto cell: testContainer.getBoundaryCells()) {
     EXPECT_EQ(cell.get().getParticlesInCell().size(), 0);
}

handler.handleOutflow();

for (auto cell: testContainer.getHaloCells()) {
     EXPECT_EQ(cell.get().getParticlesInCell().size(), 0);
}

for (auto cell: testContainer.getCells()) {
     EXPECT_EQ(cell.getParticlesInCell().size(), 0);
}

EXPECT_EQ(testContainer.getParticles().size(), 0);
}

/**Test the function handleReflecting() from BoundaryHandler */
TEST(BoundaryHandlerTest, conditionReflecting) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1, false);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);

ParticleIdInitializer::reset();

testContainer.addParticle(Particle({2, 3, 0}, {0, 0.1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -0.1, 0}, 1, 0));
testContainer.addParticle(Particle({1, 2, 0}, {-0.1, 0, 0}, 1, 0));
testContainer.addParticle(Particle({3, 2, 0}, {0.1, 0, 0}, 1, 0));

EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.01,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);


EXPECT_EQ(testContainer.getParticles().size(), 4);


while (testContainer.getParticles().at(0).getV()[1] > 0) {
     calculator.calculateXFV(testContainer, 0.01,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);
}

EXPECT_TRUE(testContainer.getParticles().at(0).getV()[1] < 0);
EXPECT_TRUE(testContainer.getParticles().at(1).getV()[1] > 0);
EXPECT_TRUE(testContainer.getParticles().at(2).getV()[0] > 0);
EXPECT_TRUE(testContainer.getParticles().at(3).getV()[0] < 0);

for (int i = 0; i < 1; i++){
calculator.calculateXFV(testContainer, 0.01,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler); 

EXPECT_EQ(testContainer.getParticles().size(), 4);
}
}

/**Test the functionionality to set different conditions for diffeent boundaries */
TEST(BoundaryHandlerTest, conditionCombine) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1, false);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);

ParticleIdInitializer::reset();

testContainer.addParticle(Particle({1, 2, 0}, {-1, 0, 0}, 1, 0)); //left
testContainer.addParticle(Particle({3, 2, 0}, {1, 0, 0}, 1, 0)); //right
testContainer.addParticle(Particle({2, 3, 0}, {0, 1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -1, 0}, 1, 0));


EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.01, 0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

EXPECT_EQ(testContainer.getParticles().size(), 4);
calculator.calculateXFV(testContainer,  0.5, 0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

do {
     calculator.calculateXFV(testContainer, 0.01,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler); 
} while (testContainer.getParticles().at(0).getV()[0] > 0);


EXPECT_TRUE(testContainer.getParticles().at(0).getV()[0] < 0);
EXPECT_TRUE(testContainer.getParticles().at(1).getV()[0] > 0);
EXPECT_EQ(testContainer.getParticles().size(), 4); //when 1 and 2 get repelled, 3 and 4 have not been removed yet

bool outflowed = false; 

for (int i = 0; i < 100; i++){
calculator.calculateXFV(testContainer, 0.01,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);


if (testContainer.getParticles().size() == 2) {
     outflowed = true;
}


if (outflowed) {
     EXPECT_TRUE(testContainer.getParticles().size() == 2); //2 particles stay until the end

     for (int a = 1; a < 3; a++) {
     bool contains = false;
     for (auto cell : testContainer.getCells()){
          for (auto p : cell.getParticlesInCell()) {
               if (p->getID() == a) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains);
     }

}
}

EXPECT_TRUE(outflowed);
}

/**Test the function handlePeriodic() from BoundaryHandler */
TEST(BoundaryHandlerTest, conditionPeriodicParticles) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1, false);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({1.1, 2.9, 0}, {0, 1.5, 0}, 1, 0)); //moves up
testContainer.addParticle(Particle({2.9, 1.2, 0}, {0, -1.5, 0}, 1, 0)); //moves down
testContainer.addParticle(Particle({1.2, 1.2, 0}, {-1.5, 0, 0}, 1, 0)); //moves left
testContainer.addParticle(Particle({2.9, 2.9, 0}, {1.5, 0, 0}, 1, 0)); //moves right

EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.5,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

EXPECT_EQ(testContainer.getParticles().size(), 4);

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

for (int i = 1; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particles are still in boundary cells
}

calculator.calculateX(testContainer, 0.5);
testContainer.updateParticlesInCell();
EXPECT_EQ(testContainer.getParticles().size(), 4);

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

for (int i = 1; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particles are in halo cells
}

handler.handlePeriodicMoveParticles();
EXPECT_EQ(testContainer.getParticles().size(), 4);

for (int i = 1; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particles are in boundary cells
}

}

TEST(BoundaryHandlerTest, conditionPeriodicMoveParticles) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1, false);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
ParticleIdInitializer::reset();

std::array<int,3> comp = {0, 0, 0};

testContainer.addParticle(Particle({0.5, 0.5, 0.5}, {0, 0, 0}, 1, 0)); 

EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(0))->getPosition());

handler.handlePeriodicMoveParticles();

EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(0))->getPosition());

// particle in halo cells

testContainer.addParticle(Particle({-0.5, 0.5, 0.5}, {0, 0, 0}, 1, 0)); 
testContainer.updateParticlesInCell();
comp = {-1, 0, 0};
EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(1))->getPosition());

handler.handlePeriodicMoveParticles();
comp = {3, 0, 0};
EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(1))->getPosition());

//particle in halo corner

testContainer.addParticle(Particle({-0.5, -0.5, 0.5}, {0, 0, 0}, 1, 0)); 
testContainer.updateParticlesInCell();
comp = {-1, -1, 0};
EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(2))->getPosition());

handler.handlePeriodicMoveParticles();
comp = {3, 3, 0};
EXPECT_EQ(comp, testContainer.mapParticleToCell(testContainer.getParticles().at(2))->getPosition());
}


TEST(BoundaryHandlerTest, conditionPeriodicAddForces){
     ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({30, 30, 30}, 10, false);
     BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC}, testContainer);
     Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
     ParticleIdInitializer::reset();

     for (auto cell: testContainer.getInnerCells()){ //add 8 particles in every inner cell
          auto pos = cell.get().getPosition();
          testContainer.addParticle(Particle({10 * (0.21 + pos[0]), 10 * (0.22 + pos[1]), 10 * (0.21 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.22 + pos[0]), 10 * (0.21 + pos[1]), 10 * (0.74 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.23 + pos[0]), 10 * (0.74 + pos[1]), 10 * (0.23 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.24 + pos[0]), 10 * (0.78 + pos[1]), 10 * (0.76 + pos[2])}, {0, 0, 0}, 1, 0));
          testContainer.addParticle(Particle({10 * (0.75 + pos[0]), 10 * (0.22 + pos[1]), 10 * (0.23 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.76 + pos[0]), 10 * (0.26 + pos[1]), 10 * (0.76 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.77 + pos[0]), 10 * (0.73 + pos[1]), 10 * (0.27 + pos[2])}, {0, 0, 0}, 1, 0)); 
          testContainer.addParticle(Particle({10 * (0.78 + pos[0]), 10 * (0.75 + pos[1]), 10 * (0.73 + pos[2])}, {0, 0, 0}, 1, 0)); 
     }

     calculator.calculateXFV(testContainer, 0.5,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

     for (auto cell: testContainer.getInnerCells()){

          for (int i = 0; i < 8; i++){


          auto comp = testContainer.getCells().at(testContainer.cellIndex(1, 1, 1)).getParticlesInCell().at(i)->getF();
          auto particleF = cell.get().getParticlesInCell().at(i)-> getF();

         EXPECT_NEAR(comp[0], particleF[0], 0.0001);
         EXPECT_NEAR(comp[1], particleF[1], 0.0001);
         EXPECT_NEAR(comp[2], particleF[2], 0.0001);

     }
     }
    

     for (auto c : testContainer.getHaloCells()) {
          EXPECT_EQ(c.get().getParticlesInCell().size(), 0);
     }
}

//Tests that boundaryHandler adds forces correctly if not all boundaries are periodic
TEST(BoundaryHandlerTest, conditionPeriodicAddForcesNotAllPeriodic){
     ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({3, 3, 3}, 1, false);
     BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
     Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
     ParticleIdInitializer::reset();

     testContainer.addParticle(Particle({0.5, 0.5, 0.5}, {0, 0, 0}, 1, 0)); 
     testContainer.addParticle(Particle({2.5, 0.5, 0.5}, {0, 0, 0}, 1, 0)); //will be influenced, left boundary
     testContainer.addParticle(Particle({0.5, 2.5, 0.5}, {0, 0, 0}, 1, 0)); //will be influenced, bottom boundary

     testContainer.addParticle(Particle({0.5, 0.5, 2.5}, {0, 0, 0}, 1, 0)); //will not be influenced, back boundary

     calculator.calculateXFV(testContainer, 0.5,  0.0, false, 0.0, 0.0, 0.0, 0,  1,  1, &handler);

     std::array<double,3> comp = {0, 0, 0};

     for (auto p: testContainer.getParticles()){
          if(p.getID() == 4){
               EXPECT_EQ(p.getF(), comp);
          } else {
               EXPECT_NE(p.getF(), comp);
          }
     }
}

