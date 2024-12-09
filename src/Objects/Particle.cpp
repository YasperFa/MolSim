/*
 * Particle.cpp
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */
#include "spdlog/spdlog.h"
#include "Particle.h"
#include <iostream>
#include "utils/ArrayUtils.h"
#include <exception>

Particle::Particle(int type_arg) {
  type = type_arg;
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
  old_x = {NAN, NAN, NAN}; //for initial periodic boundary handling
  id = ParticleIdInitializer::getNewId();
  epsilon = 5;
  sigma = 1;
  SPDLOG_DEBUG("Empty Particle with id {} generated!", id);
}

Particle::Particle(const Particle &other) {
  x = other.x;
  v = other.v;
  f = other.f;
  old_f = other.old_f;
  old_x = other.old_x;
  m = other.m;
  type = other.type;
  id = other.id;
  sigma = other.sigma;
  epsilon = other.epsilon;
  SPDLOG_DEBUG("Particle with id {} generated by copy!", id);
}

// Todo: maybe use initializater list instead of copy?
Particle::Particle(std::array<double, 3> x_arg, std::array<double, 3> v_arg,
                   double m_arg, int type_arg, double epsilon_arg, double sigma_arg) : x(x_arg), v(v_arg), m(m_arg), type(type_arg), epsilon(epsilon_arg), sigma(sigma_arg){
  f = {0., 0., 0.};
  old_f = {0., 0., 0.};
  old_x = {NAN, NAN, NAN};
  id = ParticleIdInitializer::getNewId();
  SPDLOG_DEBUG("Particle with id {} generated by value!", id);
}

Particle::~Particle() {SPDLOG_DEBUG("Particle destructed!");}

const std::array<double, 3> &Particle::getX() const { return x; }

const std::array<double, 3> &Particle::getOldX() const { return old_x; }

const std::array<double, 3> &Particle::getV() const { return v; }

const std::array<double, 3> &Particle::getF() const { return f; }

const std::array<double, 3> &Particle::getOldF() const { return old_f; }

int Particle::getID() const { return id; }

double Particle::getM() const { return m; }

int Particle::getType() const { return type; }

double Particle::getEpsilon() {return epsilon;}

double Particle::getSigma() {return sigma;}

void Particle::setF(const std::array<double, 3> &newF) {
  f = newF;
}

void Particle::setOldF(const std::array<double, 3> &newOldF) {
  old_f = newOldF;
}
void Particle::setX(const std::array<double, 3> &X) {
  x = X;
}

void Particle::setOldX(const std::array<double, 3> &newOldX) {
  old_x = newOldX;
}

void Particle::setV(const std::array<double, 3> &V) {
  v = V;
}



std::string Particle::toString() const {
  std::stringstream stream;
  stream << "Particle: X:" << x << " v: " << v << " f: " << f
         << " old_f: " << old_f << " type: " << type << " id: " << id;
  return stream.str();
}

bool Particle::operator==(Particle &other) {
  return (x == other.x) and (v == other.v) and (f == other.f) and
         (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

bool Particle::operator==(const Particle &other) {
  return (x == other.x) and (v == other.v) and (f == other.f) and
         (type == other.type) and (m == other.m) and (old_f == other.old_f);
}

std::ostream &operator<<(std::ostream &stream, Particle &p) {
  stream << p.toString();
  return stream;
}

