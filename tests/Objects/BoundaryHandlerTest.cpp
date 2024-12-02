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
BoundaryHandler handler = BoundaryHandler(1, {0, 0, 0, 0, 0, 0}, testContainer);
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
          for (Particle* p : cell.get().getParticlesInCell()) {
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
          for (Particle* p : cell.get().getParticlesInCell()) {
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
BoundaryHandler handler = BoundaryHandler(1, {1, 1, 1, 1, 1, 1}, testContainer);
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
BoundaryHandler handler = BoundaryHandler(1, {1, 1, 0, 0, 1, 1}, testContainer);
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
          for (Particle* p : cell.getParticlesInCell()) {
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

