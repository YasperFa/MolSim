//
// Created by Yasmine Farah on 22/11/2024.
//

#pragma once
#include <array>

/**
 * a class that defines disc parameters
 */
class Disc {
private:
 /*
  * coordinate of the center
  */
  std::array<double, 3> centerCoordinate;
 /*
  * Initial velocity of the whole disc
  */
 std::array<double,3> initVelocity;
 /*
  * radius in terms of the number of molecules along the radius
  */
  int radius;
 /*
  * distance between molecules
  */
  double h;
 /*
 * mass of molecules
 */
  double m;

public:
 /**
  * @brief Constructor of disc parameters
  * @param centerCoordinate: coordinate of the center of the disc
  * @param initVelocity: initial velocity of the whole disc
  * @param radius: radius in terms of the number of molecules along the radius
  * @param h: distance between molecules
  */
 Disc(const  std::array<double, 3> centerCoordinate, const std::array<double,3> initVelocity, const int radius, const double h, const double m):
centerCoordinate(centerCoordinate), initVelocity(initVelocity), radius(radius), h(h), m(m) {}

 /**
  * @return center coordinate of the disc
  */
  std::array<double, 3> getCenterCoordinate() const {
  return centerCoordinate;
 }

 /**
  * @return initial velocity of the whole disc
  */
 std::array<double, 3> getInitVelocity() const {
  return initVelocity;
 }

 /**
  * @return the radius in terms of the number of molecules along the radius
  */
 int getRadius() const {
  return radius;
 }

 /**
  * @return distance between molecules
  */
 double getDistanceBetweenParticles() const {
  return h;
 }
 /**
  * @return mass of molecules
  */
 double getMass() const {
  return m;
 }
};



