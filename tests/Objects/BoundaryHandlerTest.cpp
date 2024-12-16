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
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({2, 3, 0}, {0, 1.1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -1.1, 0}, 1, 0));
testContainer.addParticle(Particle({1, 2, 0}, {-1.1, 0, 0}, 1, 0));
testContainer.addParticle(Particle({3, 2, 0}, {1.1, 0, 0}, 1, 0));

testContainer.updateParticlesInCell();

EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.5);
handler.handleBoundaries();

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

calculator.calculateXFV(testContainer, 0.5);

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
     //SPDLOG_INFO("Checkpoint");
}

for (auto cell: testContainer.getBoundaryCells()) {
     EXPECT_EQ(cell.get().getParticlesInCell().size(), 0);
}

//SPDLOG_INFO("Checkpoint2");

handler.handleBoundaries();

for (auto cell: testContainer.getHaloCells()) {
     EXPECT_EQ(cell.get().getParticlesInCell().size(), 0);
}

for (auto cell: testContainer.getCells()) {
     EXPECT_EQ(cell.getParticlesInCell().size(), 0);
}

//SPDLOG_INFO("Checkpoint3");

EXPECT_EQ(testContainer.getParticles().size(), 0);
}

/**Test the function handleReflecting() from BoundaryHandler */
TEST(BoundaryHandlerTest, conditionReflecting) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();

ParticleIdInitializer::reset();

testContainer.addParticle(Particle({2, 3, 0}, {0, 0.1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -0.1, 0}, 1, 0));
testContainer.addParticle(Particle({1, 2, 0}, {-0.1, 0, 0}, 1, 0));
testContainer.addParticle(Particle({3, 2, 0}, {0.1, 0, 0}, 1, 0));

EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.01);
handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 4);
calculator.calculateXFV(testContainer, 0.5);
handler.handleBoundaries();

while (testContainer.getParticles().at(0).getV()[1] > 0) {
     calculator.calculateXFV(testContainer, 0.01); 
     handler.handleBoundaries();
}

EXPECT_TRUE(testContainer.getParticles().at(0).getV()[1] < 0);
EXPECT_TRUE(testContainer.getParticles().at(1).getV()[1] > 0);
EXPECT_TRUE(testContainer.getParticles().at(2).getV()[0] > 0);
EXPECT_TRUE(testContainer.getParticles().at(3).getV()[0] < 0);

for (int i = 0; i < 100; i++){
calculator.calculateXFV(testContainer, 0.01); 
handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 4);
}
}

/**Test the functionionality to set different conditions for diffeent boundaries */
TEST(BoundaryHandlerTest, conditionCombine) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::REFLECTING, BoundaryHandler::bCondition::REFLECTING}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();

ParticleIdInitializer::reset();

testContainer.addParticle(Particle({1, 2, 0}, {-1, 0, 0}, 1, 0)); //left
testContainer.addParticle(Particle({3, 2, 0}, {1, 0, 0}, 1, 0)); //right
testContainer.addParticle(Particle({2, 3, 0}, {0, 1, 0}, 1, 0));
testContainer.addParticle(Particle({2, 1, 0}, {0, -1, 0}, 1, 0));


EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.01);
handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 4);
calculator.calculateXFV(testContainer, 0.5);
handler.handleBoundaries();

do {
     calculator.calculateXFV(testContainer, 0.01); 
     handler.handleBoundaries();
} while (testContainer.getParticles().at(0).getV()[0] > 0);


EXPECT_TRUE(testContainer.getParticles().at(0).getV()[0] < 0);
EXPECT_TRUE(testContainer.getParticles().at(1).getV()[0] > 0);
EXPECT_EQ(testContainer.getParticles().size(), 4); //when 1 and 2 get repelled, 3 and 4 have not been removed yet

bool outflowed = false; 

for (int i = 0; i < 100; i++){
calculator.calculateXFV(testContainer, 0.01); 
handler.handleBoundaries();


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
TEST(BoundaryHandlerTest, conditionPeriodic) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC}, testContainer);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({1.1, 2.9, 0}, {0, 1.5, 0}, 1, 0)); //moves up
testContainer.addParticle(Particle({2.9, 1.2, 0}, {0, -1.5, 0}, 1, 0)); //moves down
testContainer.addParticle(Particle({1.2, 1.2, 0}, {-1.5, 0, 0}, 1, 0)); //moves left
testContainer.addParticle(Particle({2.9, 2.9, 0}, {1.5, 0, 0}, 1, 0)); //moves right

testContainer.updateParticlesInCell();

EXPECT_EQ(testContainer.getParticles().size(), 4);

calculator.calculateXFV(testContainer, 0.5);

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

EXPECT_EQ(testContainer.getParticles().size(), 4);

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

handler.handleBoundaries();

for (auto p : testContainer.getParticles()){
     SPDLOG_DEBUG(p.toString());
}

EXPECT_EQ(testContainer.getParticles().size(), 8);

for (int i = 5; i < 9; i++) {
     bool contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //cloned particles are in boundary cells
}

calculator.calculateXFV(testContainer, 0.5);

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

for (int i = 5; i < 9; i++) {
     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //cloned particles are in boundary cells
}

handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 8); //clone particles should not be created for particles that move into boundary cell from halo cells


}

/**Test that 3 clone particles get created for a corner particle*/
TEST(BoundaryHandlerTest, PeriodicThreeClonesForCorner) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
ParticleIdInitializer::reset();

BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC}, testContainer);

testContainer.addParticle(Particle({0.5, 0.5, 0}, {0, 1.5, 0}, 1, 0)); //corner particle

testContainer.updateParticlesInCell();

EXPECT_EQ(testContainer.getParticles().size(), 1);


     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 1) {
                    contains = true;
               }
          }
     EXPECT_TRUE(contains); //particle is in boundary cells
}

handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 4);

for (int i = 2; i < 5; i++) {
     bool contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == i) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //3 clone particles created
}
}

/**Test that only one clone particle gets created for a corner particle if not both boundaries are periodic*/
TEST(BoundaryHandlerTest, PeriodicOneCloneForCorner) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::OUTFLOW, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC}, testContainer);
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({0.5, 0.5, 0}, {0, 1.5, 0}, 1, 0)); //corner particle

testContainer.updateParticlesInCell();

EXPECT_EQ(testContainer.getParticles().size(), 1);


     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 1) {
                    contains = true;
               }
          }
     EXPECT_TRUE(contains); //particle is in boundary cells
}

handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 2);


     contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 2) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); // clone particle created

}

/**Test that only one clone particle gets created if a particle is in the boundary cells for several iterations*/
TEST(BoundaryHandlerTest, OnlyCloneOnce) {
ParticleContainers::LinkedCellContainer testContainer = ParticleContainers::LinkedCellContainer({4, 4, 1}, 1);
Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator();
BoundaryHandler handler = BoundaryHandler({BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC, BoundaryHandler::bCondition::PERIODIC}, testContainer);
ParticleIdInitializer::reset();

testContainer.addParticle(Particle({0.5, 2, 0}, {0, 0.1, 0}, 1, 0)); //corner particle

testContainer.updateParticlesInCell();

EXPECT_EQ(testContainer.getParticles().size(), 1);



     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){

          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 1) {
                    contains = true;
               }
          }
}

 EXPECT_TRUE(contains); //particle is in boundary cells

handler.handleBoundaries();

EXPECT_EQ(testContainer.getParticles().size(), 2);


     contains = false;
     for (auto cell : testContainer.getHaloCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 2) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); // clone particle created


for (int i = 0; i < 10; i++){

     calculator.calculateXFV(testContainer, 0.5);
     handler.handleBoundaries();


     bool contains = false;
     for (auto cell : testContainer.getBoundaryCells()){
          for (auto p : cell.get().getParticlesInCell()) {
               if (p->getID() == 1) {
                    contains = true;
               }
          }
     }
     EXPECT_TRUE(contains); //particle is still in boundary cells

     EXPECT_EQ(testContainer.getParticles().size(), 2); //no more clone particle gets created

}
}

