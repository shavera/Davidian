//
// Created by Alex Shaver on 10/8/17.
//

#ifndef DAVIDIAN_NEWTONSOLVER_H
#define DAVIDIAN_NEWTONSOLVER_H

#include <functional>

namespace engine {
/// @param tolerance Multiplicative factor to be a limit; eg, if last solved value is x, the equation will terminate
/// after the difference is below tolerance*x; unless last x was 0, then this is an absolute limit
double solveEquation(const std::function<double(double)>& f,
                     const std::function<double(double)>& derivative,
                     double tolerance = 1e-6,
                     double x0 = 0.0);

} // namespace engine

#endif //DAVIDIAN_NEWTONSOLVER_H
