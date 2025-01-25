//
// Created by Yasmine Farah on 01/11/2024.
//
#include "gtest/gtest.h"
#include "../../src/Objects/Containers/DirectSum/DirectSumContainer.h"
#include "../src/Objects/Particle.h"
#include "Objects/Containers/LinkedCell/LinkedCellContainer.h"
#include "spdlog/spdlog.h"
#include "Calculator/LennardJonesCalculator.h"
#include "utils/ArrayUtils.h"

/** Checks if sizeParticles() and addParticle() of DirectSumContainer work correctly */
TEST(DirectSumContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::DirectSumContainer testContainer;
    Particle p(0);
    
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 1);

    testContainer.removeParticle(p);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    

}

/** Checks if sizeParticles() and addParticle() of LinkedCellContainer work correctly and that particles are inserted in the correct cells*/
TEST(LinkedCellContainerTest, StrctureAfterAddParticle) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{8,8,1}, 1.0, false);
    Particle i({-1, -1, 0.0},{0.0, 0.0, 0.0},1.0,0);
    EXPECT_EQ(testContainer.sizeParticles(), 0);
    testContainer.addParticle(i);
    EXPECT_EQ(testContainer.sizeParticles(), 1);
    testContainer.removeParticle(i);
    EXPECT_EQ(testContainer.sizeParticles(), 0);

    ParticleIdInitializer::reset();

    for (int i = 1; i < 11; i++){

    Particle particle({-1, i - 1.5, 0.0},{0.0, 0.0, 0.0}, 1.0, 0);
    testContainer.addParticle(particle);
    EXPECT_EQ(testContainer.sizeParticles(), i);

    bool contains = false;
    std::vector<Particle*> particlesInCell = testContainer.getCells().at(i - 1).getParticlesInCell();
    for (auto p : particlesInCell){
        if (p->getID() == i){
            contains = true;
            break;
        }
    }

    EXPECT_TRUE(contains);
}
}

/** Checks that cells in the LinkedCellContainer are initialized correctly for 2D simulations*/
TEST(LinkedCellContainerTest, correctCellInitialization2D) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{180,90,1}, 3.0, false);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[0], 60);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[1], 30);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[2], 1);
    // 62*32*1 = 1984
    EXPECT_EQ(testContainer.getCells().size(), 1984);
    // 2*60 + 2*30 - 4
    EXPECT_EQ(testContainer.getBoundaryCells().size(), 176);
    // 60*30
    EXPECT_EQ(testContainer.getInnerCells().size(), 1800);
    EXPECT_EQ(testContainer.getHaloCells().size(), 184);
}
/** Checks that cells in the LinkedCellContainer are initialized correctly for 2D simulations version2*/
TEST(LinkedCellContainerTest, correctCellInitialization2DVersion2) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{180,90,1}, 3.0, true);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[0], 60);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[1], 30);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[2], 1);
    std::vector<std::vector<Cell*>> orders = testContainer.getIterationOrders();
    int orderSize = orders.size();
    EXPECT_EQ(orderSize, 9);
    int cellsNumber = 0;
    for (auto p : orders) {
        cellsNumber+= p.size();
    }
    // 62*32*1 = 1984
    EXPECT_EQ(cellsNumber, 1984);
}
//checks if order initialization splits the orders correctly
TEST(LinkedCellContainerTest, correctOrderInitialization2DVersion2) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{1,1,1}, 1.0, true);
    std::vector<std::vector<Cell*>> orders = testContainer.getIterationOrders();
    EXPECT_EQ(testContainer.getCellNumPerDimension()[0], 1);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[1], 1);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[2], 1);
    int orderSize = orders.size();
    EXPECT_EQ(orderSize, 9);
    int cellsNumber = 0;
    for (auto p : orders) {
        cellsNumber+= p.size();
    }
    // 3*3*1 = 9
    EXPECT_EQ(cellsNumber, 9);

    bool orderCorrect = (orders[0][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,-1,0))) && (orders[1][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,0,0)))
    && (orders[2][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,1,0))) && (orders[3][0] == &testContainer.getCells().at(testContainer.cellIndex(0,-1,0))) &&
        (orders[4][0] == &testContainer.getCells().at(testContainer.cellIndex(0,0,0))) && (orders[5][0] == &testContainer.getCells().at(testContainer.cellIndex(0,1,0))) &&
            (orders[6][0] == &testContainer.getCells().at(testContainer.cellIndex(1,-1,0))) && (orders[7][0] == &testContainer.getCells().at(testContainer.cellIndex(1,0,0))) && (orders[8][0] == &testContainer.getCells().at(testContainer.cellIndex(1,1,0)));
    EXPECT_TRUE(orderCorrect);

}
TEST(LinkedCellContainerTest, correctOrder2Initialization2DVersion2) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{2,2,1}, 1.0, true);
    std::vector<std::vector<Cell*>> orders = testContainer.getIterationOrders();
    EXPECT_EQ(testContainer.getCellNumPerDimension()[0], 2);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[1], 2);
    EXPECT_EQ(testContainer.getCellNumPerDimension()[2], 1);
    int orderSize = orders.size();
    EXPECT_EQ(orderSize, 9);
    int cellsNumber = 0;
    for (auto p : orders) {
        cellsNumber+= p.size();
    }
    // 3*3*1 = 9
    EXPECT_EQ(cellsNumber, 16);

    bool order1Correct = (orders[0][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,-1,0))) && (orders[0][1] == &testContainer.getCells().at(testContainer.cellIndex(-1,2,0)))
    && (orders[0][2] == &testContainer.getCells().at(testContainer.cellIndex(2,-1,0))) && (orders[0][3] == &testContainer.getCells().at(testContainer.cellIndex(2,2,0)));
    EXPECT_TRUE(order1Correct);
    bool order2Correct = (orders[1][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,0,0))) && (orders[1][1] == &testContainer.getCells().at(testContainer.cellIndex(2,0,0)));
    EXPECT_TRUE(order2Correct);
    bool order3Correct = (orders[2][0] == &testContainer.getCells().at(testContainer.cellIndex(-1,1,0))) && (orders[2][1] == &testContainer.getCells().at(testContainer.cellIndex(2,1,0)));
    EXPECT_TRUE(order3Correct);
    bool order4Correct = (orders[3][0] == &testContainer.getCells().at(testContainer.cellIndex(0,-1,0))) && (orders[3][1] == &testContainer.getCells().at(testContainer.cellIndex(0,2,0)));
    EXPECT_TRUE(order4Correct);
    bool order5Correct = (orders[4][0] == &testContainer.getCells().at(testContainer.cellIndex(0,0,0)));
    EXPECT_TRUE(order5Correct);
    bool order6Correct = (orders[5][0] == &testContainer.getCells().at(testContainer.cellIndex(0,1,0)));
    EXPECT_TRUE(order6Correct);
    bool order7Correct = (orders[6][0] == &testContainer.getCells().at(testContainer.cellIndex(1,-1,0))) && (orders[6][1] == &testContainer.getCells().at(testContainer.cellIndex(1,2,0)));
    EXPECT_TRUE(order7Correct);
    bool order8Correct = (orders[7][0] == &testContainer.getCells().at(testContainer.cellIndex(1,0,0)));
    EXPECT_TRUE(order8Correct);
    bool order9Correct = (orders[8][0] == &testContainer.getCells().at(testContainer.cellIndex(1,1,0)));
    EXPECT_TRUE(order9Correct);
}

/** Checks that only neighboring cells are influenced during calculations */
TEST(LinkedCellContainerTest, correctCalculations) {
    ParticleContainers::LinkedCellContainer testContainer(std::array<double,3>{5, 5, 1}, 1.0, false);
    Calculators::LennardJonesCalculator calculator = Calculators::LennardJonesCalculator(false);
    

    Particle p({0, 2, 0.0},{0.0, 0.0, 0.0},2.0,0);
    testContainer.addParticle(p);
    Particle q({0, 3, 0.0},{0.0, 0.0, 0.0},1.0,0); //influenced by two
    testContainer.addParticle(q);
    Particle r({0, 4, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(r);

    Particle s({4, 4, 0.0},{0, 0, 0},1.0,0); //isolated
    testContainer.addParticle(s);

    Particle t({4, 0, 0.0},{0.0, 0.0, 0.0},1.0,0); //pair that influences each other
    testContainer.addParticle(q);
    Particle u({3, 0, 0.0},{0.0, 0.0, 0.0},1.0,0);
    testContainer.addParticle(r);

    calculator.calculateXFV(testContainer, 1, 0);

    EXPECT_EQ(q.getF(), -1 * (r.getF() + p.getF()));

    std::array<double, 3> expected = {0, 0, 0};
    EXPECT_EQ(s.getF(), expected);

    EXPECT_EQ(t.getF(), -1 * (u.getF()));
    
}

/* Tests that ids of new particles get initialized correctly */
TEST(ParticleIdInitializerTest, testIDsAreUnique) {
    ParticleIdInitializer::reset();
    Particle p(1);
    EXPECT_EQ(p.getID(), 1);

    Particle q(1);
    EXPECT_EQ(q.getID(), 2);

    Particle r = Particle(p); //creating a particle from copy should copy the id
    EXPECT_EQ(r.getID(), 1);

    Particle s(1); //creating particles from copy should not influence new ids
    EXPECT_EQ(s.getID(), 3);
}