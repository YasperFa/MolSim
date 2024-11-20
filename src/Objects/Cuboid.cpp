//
// Created by Sawsan Farah on 03/11/2024.
//

#include "spdlog/spdlog.h"
#include "Cuboid.h"

Cuboid::Cuboid(const std::array<double, 3> x, const std::array<double, 3> N, const double h, const double m,
    const std::array<double, 3> v,const double mv)
    : x(x), N(N), h(h), m(m), v(v), mv(mv){}


