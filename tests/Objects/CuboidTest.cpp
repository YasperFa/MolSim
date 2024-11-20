//
// Created by Y Farahasmine on 12/11/2024.
//
#include <gtest/gtest.h>

#include "../../src/Objects/Cuboid.h"

/* tests if getters and constructor work correctly */
TEST(CuboidTest, testGettersAndConstructor) {
    std::array<double,3> x = {4.0,5.0,6.0} ;
    std::array<double,3> N = {3.0, 2.0, 1.0} ;
    std::array<double,3> V = {10.0, 0.0, 0.0} ;
    double h = 1.0 ;
    double m = 1.5 ;
    double mv = 0.5 ;
    Cuboid cuboid1(x,N, h, m,V,mv);
    EXPECT_EQ(1.0, cuboid1.h);
    EXPECT_EQ(1.5, cuboid1.m);
    EXPECT_EQ(0.5, cuboid1.mv);
    //check x
    EXPECT_EQ(4.0, cuboid1.x[0]);
    EXPECT_EQ(5.0, cuboid1.x[1]);
    EXPECT_EQ(6.0, cuboid1.x[2]);
    //check N
    EXPECT_EQ(3.0, cuboid1.N[0]);
    EXPECT_EQ(2.0, cuboid1.N[1]);
    EXPECT_EQ(1.0, cuboid1.N[2]);
    //check v
    EXPECT_EQ(10.0, cuboid1.v[0]);
    EXPECT_EQ(0.0, cuboid1.v[1]);
    EXPECT_EQ(0.0, cuboid1.v[2]);

}